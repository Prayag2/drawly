# This dockerimage can be used locally to deploy drawy to an AppImage
# Although GitHub Actions is a better way, you may use this dockerfile to create an appimage

FROM ubuntu:22.04
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
RUN ln -fs /usr/share/zoneinfo/UTC /etc/localtime

RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    wget \
    patchelf \
    fuse \
    curl \
    software-properties-common \
    libxkbcommon0 libxkbcommon-x11-0 libxkbcommon-dev \
    libxcb-keysyms1 libxcb-keysyms1-dev \
    libxcb-icccm4 libxcb-icccm4-dev \
    libgl1-mesa-dev

RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt update
RUN apt install -y python3.12
RUN curl -sS https://bootstrap.pypa.io/get-pip.py | python3.12
RUN pip3 install aqtinstall==3.3.0
RUN aqt install-qt --outputdir /opt linux desktop 6.10.0

WORKDIR /app
COPY . .

RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/opt/6.10.0/gcc_64
RUN cmake --build build --config Release

ENV LD_PRELOAD=""
ENV APPIMAGE_EXTRACT_AND_RUN=1

WORKDIR /app/deploy/appimage
RUN wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
RUN chmod +x linuxdeployqt-continuous-x86_64.AppImage

RUN apt install -y libxcb-cursor0 libxcb-cursor-dev libxcb-shape0 libxcb-shape0-dev

ENV PATH="/opt/6.10.0/gcc_64/bin/:$PATH"
RUN ./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/share/applications/drawy.desktop -appimage
