FROM debian:12-slim

ARG DEBIAN_FRONTEND=noninteractive

RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
    apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        gcc-12 g++-12 \
        cmake \
        ninja-build \
        clang-format \
        clang-tidy \
        gdb \
        ccache \
        python3 python3-venv python3-pip \
        git \
        openssh-client \
        curl \
        ca-certificates \
        pkg-config \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

RUN python3 -m venv /opt/conan-env \
    && /opt/conan-env/bin/pip install --no-cache-dir "conan>=2.0"

ENV PATH="/opt/conan-env/bin:/usr/lib/ccache:$PATH"

RUN groupadd --gid 1000 vscode \
    && useradd --uid 1000 --gid 1000 --create-home --shell /bin/bash vscode \
    && chown -R vscode:vscode /opt/conan-env

WORKDIR /workspace
RUN chown vscode:vscode /workspace

USER vscode
