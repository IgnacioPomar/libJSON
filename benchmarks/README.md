# Benchmarks: libJSON vs. otras librerías DOM

Este directorio compara `libJSON` (esta librería, DOM basado en árbol de
`shared_ptr`) contra otras librerías JSON de C++, todas usadas en su modo
**DOM** (parsean a un árbol en memoria y serializan desde él), que es el
único modo que `libJSON` soporta. No se compara contra modos SAX/streaming
ni contra el modo *on-demand* de simdjson, porque no serían comparaciones
justas frente a un diseño basado en construir siempre un árbol completo.

## Librerías comparadas

- **[nlohmann/json](https://github.com/nlohmann/json)** — la librería DOM de
  referencia en C++. API muy ergonómica (`operator[]`, conversión implícita
  de tipos). Se usa como línea base de comparación.
- **[RapidJSON](https://github.com/Tencent/rapidjson)** (modo DOM,
  `rapidjson::Document`) — usa un *arena allocator* propio, pensada para ser
  rápida sin renunciar a un árbol completo en memoria.
- **[simdjson](https://github.com/simdjson/simdjson)** (modo DOM,
  `simdjson::dom::parser`) — parser acelerado con SIMD; en modo DOM sigue
  construyendo un árbol completo, por lo que es comparable con las demás.
- **[glaze](https://github.com/stephenberry/glaze)** (tipo genérico
  `glz::json_t`) — glaze es principalmente una librería de *reflection*
  que serializa structs C++ directamente sin pasar por un árbol JSON
  intermedio (mucho más rápido, pero no es una comparación DOM-a-DOM). Para
  mantener la comparación justa usamos su tipo `json_t`, que sí es un DOM
  genérico (`std::variant` de `std::map`/`std::vector`/valores), igual que
  hacen las demás librerías. Requiere C++20, así que solo el ejecutable de
  benchmarks se compila con ese estándar (el propio `libJSON` sigue en
  C++17).

Todas se obtienen vía Conan; `libJSON` se compila directamente desde
`../src` para evitar tener que empaquetarla primero.

## Datos de prueba

Los tres tamaños usan el mismo generador (velas OHLC de 1 minuto agrupadas
por hora, mediante un paseo aleatorio donde el `close` de una vela es el
`open` de la siguiente) y solo cambian en cuántas velas incluyen:

- **Pequeño**: 10 velas minutales (10 minutos), en una sola hora.
- **Medio**: 1 día de velas minutales (24 * 60 = 1440 velas).
- **Grande**: 1 semana de velas minutales (7 * 24 * 60 = 10080 velas).

## Qué mide

Para cada librería y cada tamaño de dataset se mide, en vueltas sucesivas
(secuenciales, no intercaladas):

1. **Escritura**: construir el árbol DOM desde los datos y serializarlo a
   texto (`std::string`).
2. **Lectura**: parsear ese texto de vuelta a un árbol DOM y recorrerlo por
   completo (para evitar que el compilador optimice el parseo si no se usa
   el resultado).

Al final se imprime, por librería/dataset/operación, el tiempo mínimo,
máximo y medio de todas las vueltas.

## Cómo ejecutar

```sh
cd benchmarks
conan install . --output-folder=build --build=missing -s build_type=Release
cmake --preset conan-release
cmake --build --preset conan-release
./build/Release/json_benchmark        # o la ruta que indique el build
```

El número de vueltas se puede ajustar con el argumento
`./json_benchmark <rounds>` (por defecto 20).

## Anexo: facilidad de uso

Código real (compilable, no pseudocódigo — cada fragmento de este anexo se
ha compilado y ejecutado para comprobar que hace lo que dice) para tres
casos habituales, para valorar lo intuitiva que es cada API. `simdjson`
solo aparece en el primer caso porque es una librería de solo lectura.

### 1. Parsear JSON desde un string

**libJSON**
```cpp
JSONObject obj;
JSON_ERR_CODE err = JSONParser::parse (obj, jsonText.c_str ());
if (err != JSON_ERR_CODE::SUCCESS) { /* err es un enum con la causa */ }

int age = obj.getInt ("age");
```

**nlohmann/json**
```cpp
nlohmann::json obj = nlohmann::json::parse (jsonText);   // lanza excepción si es inválido
int age = obj["age"].get<int> ();
```

**RapidJSON**
```cpp
rapidjson::Document doc;
if (doc.Parse (jsonText.c_str ()).HasParseError ())
{
	// rapidjson::GetParseError_En (doc.GetParseError ()) da el mensaje
}
int age = doc["age"].GetInt ();
```

**glaze**
```cpp
glz::json_t obj;
if (auto ec = glz::read_json (obj, jsonText); ec) { /* ec.ec / ec.location dan el detalle */ }
int age = static_cast<int> (obj["age"].get_number ());
```

**simdjson**
```cpp
simdjson::dom::parser parser;
simdjson::dom::element doc = parser.parse (jsonText);   // lanza excepción si es inválido
int64_t age = doc["age"];                               // conversión implícita
```

Las cinco son igual de directas para este caso; la única diferencia notable
es cómo se reporta el error: excepción (nlohmann, simdjson con su API por
defecto), código de error explícito (libJSON, RapidJSON), o un `error_ctx`
que hay que comprobar (glaze).

### 2. Modificar un campo de un JSON ya en memoria

**libJSON — no hay una forma soportada.** No existe `set()`, ni `erase()`,
ni `put()` sobrescribe una clave que ya existe (véase
[`JSONObject.cpp`](../src/JSONObject.cpp): si la clave está presente, el
nuevo valor simplemente se descarta). El único camino que funciona es
hacer un `static_cast` al tipo concreto y tocar su campo público
directamente, sin ninguna comprobación de tipo segura:
```cpp
JSONObject obj;
JSONParser::parse (obj, jsonText.c_str ());

PtrJSONBase field = obj.get ("age");
if (field->getType () == JSON_TYPE::JINT)
{
	static_cast<JSONInt*> (field.get ())->value = 31;   // toca un miembro público a mano
}
// no hay forma de añadir una clave nueva que no exista todavía y luego
// "actualizarla" más tarde, salvo repitiendo este mismo patrón

std::string updated = obj.toString ();
```
(Y este `static_cast` solo compila/enlaza si `libJSON` está enlazada en
**estático**, como en este benchmark: `JSONInt` y el resto de tipos
concretos están marcados `LIBJSON_LOCAL`, visibilidad oculta, en
[`JSONBasicTypes.h`](../include/JSONBasicTypes.h) — en un build como DLL/
`.so` este workaround ni siquiera enlazaría.)

**nlohmann/json**
```cpp
json obj = json::parse (jsonText);
obj["age"] = 31;          // si "age" existe, lo sobrescribe; si no, lo crea
std::string updated = obj.dump ();
```

**RapidJSON** — modificar un valor existente y añadir uno nuevo son dos
operaciones distintas, porque `operator[]` sobre una clave que no existe es
comportamiento indefinido (no inserta, a diferencia de nlohmann/glaze):
```cpp
rapidjson::Document doc;
doc.Parse (jsonText.c_str ());
auto& alloc = doc.GetAllocator ();

doc["age"].SetInt (31);                                  // modificar: value ya existe

rapidjson::Value city;                                    // añadir: hace falta el allocator
city.SetString ("Madrid", alloc);
doc.AddMember ("city", city, alloc);
```

**glaze**
```cpp
glz::json_t obj;
glz::read_json (obj, jsonText);
obj["age"] = 31;           // igual que nlohmann: sobrescribe o crea
auto updated = glz::write_json (obj);
```

Este es el caso donde más se nota la diferencia: nlohmann y glaze resuelven
"modificar o crear" con la misma línea (`operator[] =`); RapidJSON obliga a
distinguir los dos casos y arrastrar el `Allocator`; y libJSON, tal como
está hoy, no ofrece ninguna forma pública y segura de hacerlo — es la única
de las cuatro donde este caso de uso requiere salirse de la API documentada.

### 3. Crear un JSON en memoria y volcarlo a disco

**libJSON** — no tiene una función "guardar a fichero" (solo
`parseFromFile` para leer); hay que serializar a `std::string` y escribirla
a mano:
```cpp
JSONObject obj;
obj.put ("symbol", "AAPL");
obj.put ("price", 189.5);

std::ofstream out ("quote.json");
out << obj.toString ();
```

**nlohmann/json**
```cpp
json obj;
obj["symbol"] = "AAPL";
obj["price"] = 189.5;

std::ofstream out ("quote.json");
out << obj.dump (4);       // el 4 pide indentado a 4 espacios, "gratis"
```

**RapidJSON**
```cpp
rapidjson::Document doc;
doc.SetObject ();
auto& alloc = doc.GetAllocator ();

rapidjson::Value symbolVal;
symbolVal.SetString ("AAPL", alloc);
doc.AddMember ("symbol", symbolVal, alloc);
doc.AddMember ("price", 189.5, alloc);

rapidjson::StringBuffer buffer;
rapidjson::Writer<rapidjson::StringBuffer> writer (buffer);
doc.Accept (writer);

std::ofstream out ("quote.json");
out << buffer.GetString ();
```
(RapidJSON también ofrece `FileWriteStream` para escribir directamente al
fichero sin pasar por un `std::string` intermedio — más rápido para
ficheros grandes, pero más código todavía.)

**glaze**
```cpp
glz::json_t obj;
obj["symbol"] = "AAPL";
obj["price"] = 189.5;

std::string buffer;                                    // buffer de trabajo, reutilizable
glz::write_file_json (obj, "quote.json", buffer);
```

### Valoración

| Caso | libJSON | nlohmann | RapidJSON | glaze | simdjson |
|---|---|---|---|---|---|
| Parsear string | Directo | Directo | Directo (verboso en el error) | Directo | Directo |
| Modificar campo | **No soportado por la API pública** | Directo (`operator[] =`) | Verboso (dos caminos + allocator) | Directo (`operator[] =`) | — |
| Crear + volcar a disco | Manual (sin helper de fichero) | Directo (`dump()` + `ofstream`) | Verboso (allocator en cada `AddMember`) | Directo (helper de fichero incluido) | — |

Para "parsear" las cinco están a la par. Para "crear y guardar", nlohmann y
glaze son las más cómodas (glaze incluso incluye el volcado a fichero como
función de la librería, algo que ni libJSON ni nlohmann tienen de serie).
Pero el caso que de verdad separa las opciones es "modificar un campo
existente": ahí nlohmann y glaze comparten la misma ergonomía casi perfecta
(`operator[] =` para todo), RapidJSON exige saber de antemano si la clave
existe o no y pasar el `Allocator` a mano, y `libJSON` — en su estado
actual — no tiene ninguna forma pública de hacerlo sin romper el
encapsulamiento.

### El "caso estrella" de cada librería

Los tres casos anteriores comparan tareas genéricas de DOM. Pero cada
librería tiene un truco propio que se sale de esa comparación — la razón de
ser por la que alguien la elegiría en vez de otra:

**glaze — serializar/deserializar structs directamente, sin DOM.** Este es
su modo de uso real, el que produce los números de su propio benchmark
["Roundtrip Time"](https://github.com/stephenberry/json_performance), muy
por delante de todo lo demás: con C++20, un struct normal ya es
serializable, sin macros ni heredar de nada.
```cpp
struct Quote { std::string symbol; double price; };

Quote q{"AAPL", 189.5};
auto json = glz::write_json (q);          // -> {"symbol":"AAPL","price":189.5}

Quote parsed{};
glz::read_json (parsed, *json);           // rellena parsed.symbol / parsed.price directamente
```
Nada de `put()`/`operator[]`/árboles intermedios: el struct **es** la
representación, glaze solo sabe convertirlo a/desde texto. Por eso el resto
de este documento compara contra `glz::json_t` (su modo DOM) y no contra
esto — comparar este modo con el DOM de las demás librerías no sería una
comparación justa.

**nlohmann/json — lo mismo, pero con una macro de una línea.** No tiene
reflection (funciona desde C++11), pero ofrece un atajo casi tan cómodo:
```cpp
struct Quote { std::string symbol; double price; };
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE (Quote, symbol, price)

Quote q{"AAPL", 189.5};
json j = q;                    // conversión automática, gracias a la macro
Quote back = j.get<Quote> ();
```
La macro genera por debajo las funciones `to_json`/`from_json` que nlohmann
busca por ADL. Sigue pasando por un `json` (DOM) intermedio — más lento que
glaze en este modo — pero con una sola línea de boilerplate y sin exigir
C++20.

**RapidJSON — SAX, memoria constante.** Su otro modo, además del DOM
usado en el resto de este benchmark: un `Handler` que recibe eventos
(`Int`, `Double`, `String`, `StartObject`...) a medida que el parser
avanza, sin construir nunca un árbol en memoria — ideal para ficheros que
no caben cómodamente en RAM.
```cpp
struct SumHandler : rapidjson::BaseReaderHandler<rapidjson::UTF8<>, SumHandler>
{
	double sum = 0;
	bool Double (double d) { sum += d; return true; }
	bool Int (int i) { sum += i; return true; }
};

SumHandler handler;
rapidjson::Reader reader;
rapidjson::StringStream ss (jsonText.c_str ());
reader.Parse (ss, handler);
// handler.sum ya tiene el resultado; en ningún momento hubo un Document en memoria
```

**simdjson — On-Demand, solo se toca lo que se usa.** Su modo estrella
real, descartado al principio de este documento por no ser comparable con
un DOM: no parsea ningún campo hasta que se accede a él.
```cpp
simdjson::ondemand::parser parser;
simdjson::padded_string json = simdjson::padded_string::load ("quotes.json");
simdjson::ondemand::document doc = parser.iterate (json);

double price = doc["price"];   // solo este campo se parsea de verdad
```
Si el documento tiene cien campos y solo se lee uno, simdjson no gasta
tiempo en los otros noventa y nueve — a costa de que el orden en que se
accede a los campos importa (es un parser hacia delante, no un árbol
navegable en cualquier dirección).

**libJSON — copiar es compartir, no clonar.** Es lo contrario de un "modo
más rápido": es la consecuencia directa de usar `shared_ptr` para todo. Al
copiar un `JSONObject`/`JSONArray`, la copia y el original **apuntan al
mismo almacenamiento** — mutar uno se ve a través del otro, sin pasar
referencias ni punteros explícitos. Esto permite construir una pieza,
engancharla a su contenedor, y seguir completándola después sin volver a
tocar el contenedor: por ejemplo, crear una vela (mínimo, máximo, apertura,
cierre), añadirla a la rama del árbol que corresponda, y solo entonces
rellenar la fecha:
```cpp
JSONObject candle;
candle.put ("min", 98.2);
candle.put ("max", 101.5);
candle.put ("open", 100.0);
candle.put ("close", 100.8);

JSONArray rama;
rama.put (candle);              // engancha la vela a la rama

candle.put ("date", "2026-09-22");   // se añade DESPUÉS de engancharla...

std::cout << rama.toString ();
// ...y aun así aparece dentro de "rama":
// [{"min":98.200000,"max":101.500000,"open":100.000000,"close":100.800000,"date":"2026-09-22"}]
```
`rama.put (candle)` guarda internamente una copia del *handle* `candle`,
no del contenido en ese instante — como ambas copias comparten el mismo
almacenamiento, el campo añadido después de enganchar la vela también
queda dentro de la rama. Ninguna de las otras cuatro librerías se comporta
así — en nlohmann, glaze y (en la medida en que se puede) RapidJSON,
copiar clona de verdad, así que la misma secuencia dejaría la vela dentro
de la rama sin fecha. Es una característica genuina, no necesariamente una
ventaja: es tan fácil de aprovechar (pasar el "handle" por valor sin pensar
en ownership) como de
disparar un bug por alias inesperado si no se tiene presente.
