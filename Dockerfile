FROM ubuntu:22.04

ARG USER_UID
ARG USER_GID

# 建立使用者（避免 root）
RUN groupadd -g ${USER_GID} qemu_user && \
    useradd -ms /bin/bash -u ${USER_UID} -g ${USER_GID} qemu_user

# 安裝開發工具
RUN apt update && apt install -y \
    build-essential git wget curl python3 make sudo \
    ninja-build libglib2.0-dev libpixman-1-dev libncurses5-dev libncursesw5-dev \
    gdb-multiarch python3 python3-pip vim \
    gcc-arm-linux-gnueabi g++-arm-linux-gnueabi gcc-arm-none-eabi \
    && apt clean
RUN pip3 install tomli sphinx==6.2.1 sphinx_rtd_theme

RUN apt update && apt install -y locales && \
    locale-gen en_US.UTF-8 && \
    update-locale LANG=en_US.UTF-8

ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# 加入 NOPASSWD sudo 權限
RUN echo "qemu_user ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/qemu_user && chmod 440 /etc/sudoers.d/qemu_user

# 預設進入新使用者家目錄
WORKDIR /home/qemu_user

# 安裝 SystemC
COPY systemc-2.3.3.tar.gz .
RUN tar -xzf systemc-2.3.3.tar.gz && \
    cd systemc-2.3.3 && mkdir build && cd build && \
    ../configure --prefix=/opt/systemc && \
    make -j$(nproc) && make install && \
    rm -rf systemc-2.3.3 systemc-2.3.3.tar.gz
ENV LD_LIBRARY_PATH=/opt/systemc/lib-linux64:$LD_LIBRARY_PATH
ENV SYSTEMC_HOME=/opt/systemc/

# 預設啟動 bash（但 docker-compose 已經指定 user）
CMD ["/bin/bash"]
