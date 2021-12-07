#!/bin/sh

container=`docker ps | tail -1 | awk '{print $1}'`
docker exec -it $container bash