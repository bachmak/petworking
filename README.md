# petworking

Простой асинхронный эхо-сервер на основе boost::asio.

Сервер поддерживает работу с несколькими клиентами, отправляя принятые от них сообщения обратно.

#### Процедура сборки

```
python -m pip install conan
mkdir build $$ cd build
conan install ..
conan build ..
```

#### Поддерживаемые компиляторы
gcc-9+, clang-11+
