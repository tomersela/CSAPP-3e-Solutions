FROM gcc:latest

WORKDIR /usr/src/app

COPY . .

RUN gcc -o 20 -I./include csapp.c 20.c malloc/memlib.c malloc/mm.c

CMD ["./20"]
