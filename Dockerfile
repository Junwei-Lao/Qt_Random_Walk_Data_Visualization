FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake \
    qt6-base-dev qt6-base-dev-tools qt6-tools-dev-tools \
    libgl1-mesa-dev libegl1-mesa-dev mesa-common-dev \
    libx11-6 libx11-xcb1 libxcb1 libxkbcommon0 libxrender1 libxi6 libxext6 libsm6 libice6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .
COPY src/ src/
COPY data/ data/

RUN mkdir build && cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Qt runtime only
RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    libxkbcommon-x11-0 \
    libgl1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy executable
COPY --from=builder /app/build/RandomWalkViewer /app/RandomWalkViewer
COPY --from=builder /app/data /app/data

# Qt display settings (this app was developed on wsl2 with xcb)
ENV QT_QPA_PLATFORM=xcb

CMD ["./RandomWalkViewer"]
