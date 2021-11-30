# otus-matrix
Sparse matrix implementation

### Описание

Реализована N-мерная разреженная матрица.

* Данные хранятся в хэш-таблице с ключом в виде массива `std::array<...>`.
* Для реализации оператора `[]` введен дополнительный объект `Proxy`.
```C++
template <typename M, std::size_t Dim>
class Proxy {
    ...
public:

     Proxy<self_t, Dim - 1> operator[](std::size_t);
private:
    M& m;
    ...
}

template <typename T, T Def, std::size_t Dim = 2>
class Matrix {
...
public:

    Proxy<self_t, Dim> operator[](std::size_t);

private:
    unordered_map<std::array<std::size_t>, Dim> storage;
};
```
* Для реализации `((matrix[100][100] = 314) = 0) = 217` оператор присваивания возвращает экземпляр класса `Proxy<M, 1>`;
```C++
template <typename M, std::size_t Dim>
class Proxy {
    ...
public:

     Proxy<M, Dim>& operator=(value_t&& value) {
         ...
         return *this;
     }
private:
    M& m;
    ...
}
```

### Что можно было бы улучшить

В текущей реализации хранение элементов зафиксировано в шаблоне `Matrix`. Вместо этого можно было бы реализовать хранилище в виде отдельного класса-стратегии, так чтобы клиент выбирал интересующую стратегию хранения исходя из своих нужд.

### Установка

```
mkdir build
cd build
cmake .. -DPATCH_VERSION=1
cmake --build .
./src/matrix
```
