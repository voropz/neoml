# Класс CLrnLayer

<!-- TOC -->

- [Класс CLrnLayer](#класс-clrnlayer)
    - [Настройки](#настройки)
        - [Размер окна](#поканальный-режим)
        - [Свободный член (bias)](#свободный-член-(bias))
        - [Множитель (alpha)](#множитель-(alpha))
        - [Показатель степени (beta)](#показатель-степени-(beta))
    - [Обучаемые параметры](#обучаемые-параметры)
    - [Входы](#входы)
    - [Выходы](#выходы)

<!-- /TOC -->

Класс реализует слой, вычисляющий Local Response Normalization по формуле:

```c++
LRN(x)[obj][ch] = x[obj][ch] * / ((bias + alpha * sqrSum[obj][ch] / windowSize) ^ beta)
```

где:

- `obj` - индекс объекта, принимает значения из множества `[0;BlobSize / Channels)`
- `ch` - индекс канала, принимает значения из множества `[0;Channels)`
- `windowSize`, `bias`, `alpha`, `beta` - настраиваемые параметры
- `sqrSum` вычисляется по формуле

```c++
sqrSum(x)[obj][ch] = sum(x[obj][i] * x[obj][i] for each i in [ch_min, ch_max])
ch_min = max(0, ch - floor((windowSize - 1)/2))
ch_max = min(C - 1, ch + ceil((windowSize - 1)/2))
```

## Настройки

### Размер окна

```c++
void SetWindowSize( int value );
```

Установка размер окна, на основе которого считается сумма квадратов (`sqrSum`).

### Свободный член (bias)

```c++
SetBias( float value );
```

Установка свободного члена, прибавляемого к сумме квадратов.

### Множитель (alpha)

```c++
SetAlpha( float value );
```

Установка множителя суммы квадратов.

### Показатель степени (beta)

```c++
SetBeta( float value );
```

Установка показателя степени, в которую возводится сумма квадратов.

## Обучаемые параметры

Слой не имеет обучаемых параметров.

## Входы

На единственный вход подается блоб произвольного размера.

## Выходы

Единственный выход содержит блоб того же размера, с результатами работы слоя.