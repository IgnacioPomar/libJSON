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

Las tres se obtienen vía Conan; `libJSON` se compila directamente desde
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
