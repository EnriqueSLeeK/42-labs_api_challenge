# Sumário
- [API](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#api---viddata)
    - [Introdução](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#introdu%C3%A7%C3%A3o)
    - [Dependências](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#depend%C3%AAncias)
    - [Instalando as dependencias](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#instalando-as-dependencias)
    - [Compilação e remoção](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#compila%C3%A7%C3%A3o-e-remo%C3%A7%C3%A3o)
    - [Preparando o ambiente](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#preparando-o-ambiente)
    - [Como usar o api](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#como-usar-o-api)
- [CLI - tablefy](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#cli---tablefy)
    - [Introdução](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#introdu%C3%A7%C3%A3o-1)
    - [Compilação e remoção](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#compila%C3%A7%C3%A3o-e-remo%C3%A7%C3%A3o-1)
    - [Como usar](https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK#como-usar)

# API - VidData
## Introdução
Um api simples escrito em C seguindo a arquiterura REST para o processo seletivo do 42labs

## Dependências
- Postgresql 13.6
- libpq
- Mongoose Network Library
- Mjson
- gcc (GNU compiler)

## Instalando as dependencias
### Arch linux
Instalando o postgresql e a sua biblioteca:
```
pacman -S postgresql
```
Se por algum motivo a biblioteca não for intalada podemos executar o comando:
```
pacman -S postgresql-libs
```
Instalando ferramentas básicas de desenvolvimento:
```
pacman -S base-devel
```
Para outras distribuições linux consulte o repositório oficial delas
## Compilação e remoção
Compilando
```
make api
```
Removendo
```
make clean_api
```

## Preparando o ambiente
O passos a seguir podem ser lidos no [Arch linux wiki](https://wiki.archlinux.org/title/PostgreSQL#Initial_configuration)
### Postgresql
Primeiro vamos entrar na conta que o postgresql criou o `postgres` com ele vamos criar o nosso cluster para o banco de dado
1. Primeiro mudamos a senha dele
```
# passwd posgres
```
2. Vamos dar log in
```
$ su postgres
```
3. Inicializamos o cluster com o seguinte comando
```
[postgres]$ initdb -D /var/lib/postgres/data
```
4. Iniciamos o serviço
```
$ systemctl start postgresql
```
5. Criamos um novo usuário no postgresql e vamos dar permissões para modificar tabelas (o nome dele deve ser `labs`)
```
[postgres]$ createuser --interactive
```
6. Criamos um novo banco de dados chamado `labs-db`
```
[postgres]$ createdb -O labs labs-db
```
## Como usar o api
### Endpoints
#### Operações disponíveis
|path|GET|POST|DELETE|PATCH|PUT|
|---|---|---|---|---|---|
|/|O|X|X|X|X|
|/channel|O|O|O|X|X|
|/channel/:channel_id|O|X|O|O|O|
|/channel/:channel_id/:video_id|O|X|O|O|O|
|/videos|O|O|O|X|X|
|/videos/:video_id|O|X|X|X|X|
- O channel_id e o video_id são números
### Estruturas

Caso seja necessário mandar um JSON na requisição os campos são os seguintes

#### Vídeo
|Nome|tipo|
|---|---|
|video_name|String|
|creator_id|int|
|id|int|

#### Canal
|Nome|tiop|
|---|---|
|creator_id|int|
|channel|String|
#### Exemplos de JSON
- Um video
```
{
    "video_name": "aaaa",
    "creator_id": 1,
    "id": 3
}
```
-  Um canal
```
{
    "channel": "Wow",
    "creator_id": 1
}
```
Ao mandar em uma requisição eles devem ser estruturados no formato JSON
### GET
Requisição sem corpo(JSON). Interação por meio da `URI`
### POST
Todos os campos devem ser fornecidos
### PUT
Atualizar um recurso por inteiro, todos os campos devem ser fornecidos
### PATCH
Atualização parcial de um recurso, não é necessário fornecer todos os campos
### DELETE
Requisição sem corpo(JSON). Interação por meio da `URI`

# CLI - tablefy
## Introdução
Uma ferramenta do estilo CLI para mostrar de maneira mais organizada os logs gerados pelo api
## Compilação e remoção
Compilando:
```
make cli
```
Removendo:
```
make clean_cli
```
## Como usar
O comportamento do `tablefy` vai ser procurar pelos logs no local `./Logs/log`
```
./tablefy <flag> <argumento para a flag>
```
### Flags
|Flag|Descrição|
|---|---|
|-f| Formata de maneira diferente|
|-d| procura no diretorio fornecido|
#### Flag -f
|Símbolo|Descrição|
|---|---|
|%m| O método usado|
|%e| Endpoint acessado|
|%a| O User-Agent|
|%k| O status da requisição|
|%t| Mostra o tempo que foi a requisição |
- Exemplo
```
./tablefy -f "%m %e %a"
./tablefy -f "%a"
```
#### Flag -d
- Exemplo
 ```
 ./tablefy -d "dir/to/log"
 ```
