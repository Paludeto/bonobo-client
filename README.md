# roboap-client

Cliente C++/Qt para integração com o RoboCup (FIRA/VSS). O projeto é construído com CMake, utiliza Qt6 e Protobuf para comunicação com os serviços de visão, árbitro e atuadores.

## Requisitos locais

- CMake 3.16+
- Qt 6 (Core, Network, Gui)
- Protobuf (bibliotecas e compilador)
- Compilador C++17

## Uso com Docker

### Construir a imagem

Na raiz do repositório execute:

```bash
docker build -t roboap-client .
```

### Executar o contêiner

O cliente utiliza multicast UDP para se comunicar com visão, árbitro e atuador, portanto é recomendável usar o modo de rede host em ambientes Linux:

```bash
docker run --rm --network host roboap-client
```

Caso precise sobrescrever o comportamento padrão (por exemplo, alterar endereços IP/portas ou passar variáveis de ambiente do Qt), adicione as flags correspondentes ao comando `docker run`.

### Volume opcional para logs

Se o aplicativo gerar arquivos ou logs que você queira persistir, monte um volume:

```bash
docker run --rm --network host -v $(pwd)/logs:/app/logs roboap-client
```

## Estrutura do projeto

- `proto/`: Definições `.proto` e geração da biblioteca `pb_msgs_lib`.
- `entities/`, `client/`, `utils/`: Código-fonte principal do cliente.
- `main.cpp`: Ponto de entrada da aplicação.

## Desenvolvimento local

```bash
cmake -S . -B build
cmake --build build --parallel
./build/roboap-client
```