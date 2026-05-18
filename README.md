# Cinnabar Programming Language

Cinnabar — компилируемый язык программирования, который транспилируется в Python AST для интеграции с экосистемой Python.

## Возможности

- ✅ Статическая типизация с автовыводом типов
- ✅ Семантический анализ и type checking
- ✅ Генерация в Python AST
- ✅ Поддержка базовых типов: int, float, str, char, bool
- ✅ Арифметические, логические операции и операции сравнения
- ✅ Управляющие конструкции: if/else, while
- ✅ Вызовы функций

## Синтаксис

### Объявление переменных

```cinnabar
// С явным указанием типа
x:int = 5;
name:str = "hello";
flag:bool = true;

// С автовыводом типа
y := 10;
pi := 3.14;
```

### Арифметические операции

```cinnabar
a := 10;
b := 20;
sum := a + b;
product := a * b;
power := a ** 2;  // возведение в степень
```

### Условные операторы

```cinnabar
x := 15;
if (x > 10) {
    print(x);
} else {
    print(0);
}
```

### Циклы

```cinnabar
i := 0;
while (i < 10) {
    print(i);
    i = i + 1;
}
```

### Логические операции

```cinnabar
flag := true;
x := 5;
result := flag && (x > 3);  // AND
result2 := flag || (x < 0);  // OR
negated := !flag;            // NOT
```

## Сборка

```bash
cmake -B build -S .
cmake --build build
```

## Использование

### Базовая компиляция и проверка

```bash
./build/src/Cinnabar program.cb
```

Выводит токены, AST и результаты семантического анализа.

### Генерация JSON AST

```bash
./build/src/Cinnabar program.cb --json
```

### Сохранение JSON в файл

```bash
./build/src/Cinnabar program.cb --json --output ast.json
```

### Генерация и выполнение Python кода

```bash
./build/src/Cinnabar program.cb --codegen
```

### Генерация Python кода из JSON AST

```bash
python3 codegen.py ast.json --output output.py
python3 codegen.py ast.json --execute  # выполнить сразу
```

## Примеры

### Пример 1: Вычисления

```cinnabar
x := 5;
y := 10;
z := x + y;
result := z * 2;
print(result);  // Выведет: 30
```

### Пример 2: Цикл (сумма чисел от 1 до n)

```cinnabar
n := 10;
sum := 0;
i := 1;

while (i <= n) {
    sum = sum + i;
    i = i + 1;
}

print(sum);  // Выведет: 55
```

### Пример 3: Работа со строками

```cinnabar
name:str = "hello";
greeting := name + " world";
print(greeting);  // Выведет: hello world
```

## Архитектура

Компилятор состоит из следующих компонентов:

1. **Lexer** (`src/lexer/`) - лексический анализатор, преобразует исходный код в токены
2. **Parser** (`src/parser/`) - синтаксический анализатор, строит AST из токенов
3. **Semantic Analyzer** (`src/semantic/`) - семантический анализ и type checking
4. **Code Generator** (`src/codegen/`) - сериализация AST в JSON
5. **Python Generator** (`codegen.py`) - генерация Python AST из JSON

### Типы данных

Язык поддерживает следующие типы:

- `int` - целые числа (64-bit)
- `float` - числа с плавающей точкой (double)
- `str` - строки
- `char` - символы
- `bool` - логические значения (true/false)
- `null` - null значение

### Type Checking

Семантический анализатор выполняет следующие проверки:

- Проверка объявления переменных перед использованием
- Проверка совместимости типов при присваивании
- Проверка типов операндов в арифметических операциях
- Проверка типов в логических операциях
- Автовывод типов для объявлений с `:=`

## Улучшения кода

В процессе разработки были выявлены и исправлены следующие проблемы:

### Исправленные проблемы:

1. ✅ Добавлен полноценный type checker
2. ✅ Исправлена проверка типов в бинарных операциях
3. ✅ Добавлена проверка унарных операций
4. ✅ Реализован автовывод типов для `:=`
5. ✅ Добавлена поддержка операторов `&&`, `||`, `!`
6. ✅ Разделены типы данных и токены (введен enum Type)
7. ✅ Добавлена проверка совместимости типов при присваивании

## Тестирование

Запуск тестов:

```bash
./build/tests/cinnabar_tests
```

## Лицензия

MIT License

## Автор

Cinnabar Language Project
