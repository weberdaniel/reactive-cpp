FROM ubuntu:latest
WORKDIR /project
# Copies your code file from your action repository to the filesystem path `/` of the container
COPY . .

# Code file to execute when the docker container starts up (`entrypoint.sh`)
RUN cd /project; ls -la
