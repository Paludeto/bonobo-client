# syntax=docker/dockerfile:1.6

#############################
# Build stage
#############################
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       cmake \
       ninja-build \
       qt6-base-dev \
       qt6-base-dev-tools \
       protobuf-compiler \
       libprotobuf-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY CMakeLists.txt .
COPY main.cpp .
COPY client client
COPY entities entities
COPY proto proto
COPY utils utils

RUN cmake -S . -B build -G Ninja \
    && cmake --build build --parallel

#############################
# Runtime stage
#############################
FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
     && apt-get install -y --no-install-recommends \
         qt6-base-dev \
         libprotobuf32 \
    && rm -rf /var/lib/apt/lists/*

# Qt applications may need to choose a platform plugin; offscreen works for headless
ENV QT_QPA_PLATFORM=offscreen

WORKDIR /app
COPY --from=builder /workspace/build/roboap-client /usr/local/bin/roboap-client

ENTRYPOINT ["/usr/local/bin/roboap-client"]
