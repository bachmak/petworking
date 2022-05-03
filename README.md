# petworking

Простой асинхронный эхо-сервер на основе boost::asio.

Сервер поддерживает работу с несколькими клиентами, отправляя принятые от них сообщения обратно.

#### Процедура сборки

Скачать образ с предустановленным окружением:
```
docker pull bachmak/ubuntu_gcc12_clang14
```
Создать и запустить в интерактивном режиме контейнер от образа:
```
docker run --name petworking -it bachmak/ubuntu_gcc12_clang14
```
_внутри контейнера:_
```
git clone https://github.com/bachmak/petworking.git
cd petworking && mkdir .build && cd .build
```
Скачать/собрать и подключить зависимости:
```
conan install .. --build=missing
```
Непосредственно собрать проект:
```
conan build ..
```
