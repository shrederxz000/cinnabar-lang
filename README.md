 # Cinnabar
 1. Общая идея

Cinnabar — строго типизированный язык с умеренным «сахаром» для удобного синтаксиса и совместимости с Python экосистемой.
Главное назначение: безопасное, понятное и предсказуемое программирование для проектов любого масштаба, от скриптов до enterprise.

2. Основные элементы синтаксиса
Scope
```cbar
scope test_program {...}
```


Scope — область видимости. Всё объявление и функции находятся внутри scope.

## Подключение модулей
```cbar
plug std::validator;
```

Подключает стандартные или внешние библиотеки, как Python import.

## Enum и Struct
```cbar
enum Sex {
    MALE,
    FEMALE
}
```
```cbar
struct Human {
    pub name: str;   // публичное поле
    surname: str;    // приватное поле
    sex: Sex;
    age: uint8;
}
```

`enum` — перечисления, как в Rust или C++.

`struct` — структуры с явным контролем публичности (pub) полей.

## Декораторы и автоматическая инициализация
```cbar
@fields_init(name, surname, sex, age)
pub Human(name: str, surname: str, sex: str, age: str) { ... }
```

@fields_init автоматически инициализирует указанные поля, похож на @Data или @AllArgsConstructor в Java.

## Методы и self
```cbar
pub def show_info(self) {
    printf("name: {self.name}, surname: {self.surname}, sex: {self.sex}");
}
```

self указывает на текущую структуру.

Методы могут быть pub или приватными.

## Итерации и динамическое обновление
```cbar
pub def update(self, name: str, surname: str, ...) {
    for i; i..$args.len() {
        if $args.value[i] != None {
            let matching_field = std::search_field(self, $args.field[i]);
            matching_field? = $args.value[i]: throw std::runtime_error();
        }
    }
}
```

`$args` — список аргументов, итерируемый как массив.

Динамически ищем поля структуры и обновляем их безопасно.

## Создание объектов и вызов методов
```cbar
let Alice: Human = Human("Alice", "Cross", Sex::FEMALE, 23, "USA", "+1-800-645-8832");
Alice.show_info();
```

Читаемый и понятный синтаксис для инициализации объектов.

Публичные поля доступны напрямую, приватные — только через методы.

## Особенности:

Строгая типизация (str, (u)int16/32/64/128, float32/64, char).

pub и приватность полей.

Интеграция с python экосистемой и предовсталение более предсказуемых Python инструментов

Упаковка байткода в артeфакты а так-же манифест для интеграции с внешними зависимостями и идентфикации пакета




