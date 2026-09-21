# Этап сборки
FROM gcc:latest AS builder
WORKDIR /app
COPY . .
RUN g++ -o hello HelloDockerCpp.cpp

# Этап запуска
FROM debian:stable-slim
WORKDIR /app
COPY --from=builder /app/hello .
CMD ["./hello"]
