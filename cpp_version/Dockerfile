FROM ghcr.io/osgeo/gdal:ubuntu-small-3.7.3
WORKDIR /app
COPY . .
RUN g++ main.cpp -o geotiff_to_ecw -lgdal -std=c++17
CMD ["./geotiff_to_ecw"]