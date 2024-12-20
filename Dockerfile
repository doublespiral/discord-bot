FROM alpine:latest 

RUN apk add curl-dev
RUN apk add git
RUN apk add build-base

WORKDIR /root
RUN git clone https://github.com/cogmasters/concord.git 

WORKDIR /root/concord
RUN make
RUN make install

WORKDIR /root
COPY . .
RUN mkdir bin
RUN make
RUN rm -rf bin/

WORKDIR /root/bot
CMD ["./main", "config.json", "&"]
