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
    # Additional dependencies for AWS SDK
    libssl-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Clone and build AWS SDK for C++ with only the S3 component
RUN whoami

RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git
RUN mkdir -p /aws-sdk-cpp/build
RUN chown root:root /usr/local -R && chmod 755 /usr/local -R

RUN cd /aws-sdk-cpp/build && cmake .. -DBUILD_ONLY="core;s3" -DENABLE_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_PREFIX_PATH=/usr/local -DAws-sdk-cpp_DIR=/usr/local/lib/cmake/Aws-sdk-cpp --debug-output --trace
RUN cd /aws-sdk-cpp/build && make && make install

# Debug: List the contents to verify installation
RUN ls -lah /usr/local/lib/cmake/Aws-sdk-cpp || true
RUN ls -lah /usr/local/lib/cmake || true
RUN ls -lah /usr/local || true

# Check if AWS SDK was installed successfully
# Check if AWS SDK was installed successfully
RUN if [ ! -d "/usr/local/lib/cmake/AWSSDK" ]; then echo "AWS SDK installation failed"; exit 1; fi

# Install nlohmann/json library
RUN apt-get update && apt-get install -y nlohmann-json3-dev

# Copy the entire project directory into the container
COPY . /replacer

# Build replacer
RUN mkdir -p /replacer/build && \
    cd /replacer/build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_PREFIX_PATH=/usr/local -DAws-sdk-cpp_DIR=/usr/local/lib/cmake/AWSSDK && \
    make

# Start the runner stage
FROM ubuntu:20.04 AS runner

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    tcl \
    libsm6 libx11-6 libxext6 libjpeg8 libgomp1 \
    libcurl4 \
    && rm -rf /var/lib/apt/lists/*

# Copy necessary binaries and files from the builder
COPY --from=builder /replacer/build/replace /build/replace
COPY --from=builder /replacer/module/OpenSTA/app/sta /build/sta
COPY --from=builder /replacer/test/PORT9.dat /build/share/PORT9.dat
COPY --from=builder /replacer/test/POST9.dat /build/share/POST9.dat
COPY --from=builder /replacer/test/POWV9.dat /build/share/POWV9.dat
COPY --from=builder /replacer/test/library /build/share/library
COPY --from=builder /replacer/test/design /build/share/design
COPY --from=builder /replacer/run_placement_task.sh /home/openroad/run_placement_task.sh
COPY --from=builder /replacer/run_placement.tcl /home/openroad/run_placement.tcl

# Create a non-root user and change ownership of files
RUN useradd -ms /bin/bash openroad && \
    chown -R openroad:openroad /home/openroad

USER openroad
WORKDIR /home/openroad

# Make your script executable
RUN chmod +x ./run_placement_task.sh

# Set your script as the entry point
ENTRYPOINT ["./run_placement_task.sh"]

