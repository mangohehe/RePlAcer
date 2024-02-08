# Use Ubuntu 20.04 as the base for both builder and runner stages
FROM ubuntu:20.04 AS builder

# Avoid prompts from apt-get
ARG DEBIAN_FRONTEND=noninteractive

# Install build essentials and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc g++ \
    libstdc++-9-dev \
    libx11-dev \
    libboost-all-dev \
    zlib1g-dev \
    tcl-dev tk-dev \
    swig \
    flex \
    libgmp-dev libmpfr-dev libmpc-dev \
    bison \
    imagemagick libmagick++-dev \
    git \
    cmake \
    libjpeg-turbo8-dev \
    libgomp1 \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire project directory into the container
COPY . /RePlAce

# Build RePlAce
RUN mkdir -p /RePlAce/build && \
    cd /RePlAce/build && \
    cmake -DCMAKE_INSTALL_PREFIX=/build .. && \
    make

# Start the runner stage
FROM ubuntu:20.04 AS runner

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    tcl \
    libsm6 libx11-6 libxext6 libjpeg8 libgomp1 \
    && rm -rf /var/lib/apt/lists/*

# Copy necessary binaries and files from the builder
COPY --from=builder /RePlAce/build/replace /build/replace
COPY --from=builder /RePlAce/module/OpenSTA/app/sta /build/sta
COPY --from=builder /RePlAce/test/PORT9.dat /build/share/PORT9.dat
COPY --from=builder /RePlAce/test/POST9.dat /build/share/POST9.dat
COPY --from=builder /RePlAce/test/POWV9.dat /build/share/POWV9.dat
COPY --from=builder /RePlAce/test/library /build/share/library
COPY --from=builder /RePlAce/test/design /build/share/design
COPY --from=builder /RePlAce/run_placement_task.sh /home/openroad/run_placement_task.sh
COPY --from=builder /RePlAce/run_placement.tcl /home/openroad/run_placement.tcl

# Create a non-root user and change ownership of files
RUN useradd -ms /bin/bash openroad && \
    chown -R openroad:openroad /home/openroad

USER openroad
WORKDIR /home/openroad

# Make your script executable
RUN chmod +x ./run_placement_task.sh

# Set your script as the entry point
ENTRYPOINT ["./run_placement_task.sh"]

