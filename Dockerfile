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
