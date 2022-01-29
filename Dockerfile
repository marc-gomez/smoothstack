FROM centos:latest

RUN yum update -y
RUN yum -y install gdb
RUN yum -y install vim
RUN yum -y install zip
RUN yum -y install curl
RUN yum -y install clang
RUN yum -y install epel-release
RUN yum -y install htop
RUN yum -y install iftop
RUN yum -y install iotop
RUN yum -y install cscope
RUN yum -y install tree
RUN dnf -y group install "Development Tools"
RUN dnf -y install gcc-toolset-11-gcc
RUN dnf -y install gcc-toolset-11-gcc-c++

RUN git clone https://github.com/protocolbuffers/protobuf.git
WORKDIR protobuf
RUN git submodule update --init --recursive
RUN ./autogen.sh
RUN ./configure
RUN make
RUN make check
RUN make install
RUN ldconfig
ENV PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
ENV LD_LIBRARY_PATH=/usr/local/lib
WORKDIR /smoothstack/
