# Контрольное решение СЛАУ

Нужно написать программу на примере которой можно оценить эффективность решения на технологии WebAssembly.

Решение следует написать на языке программирования C. Это позволит быстро портировать решение на технологию WebAssembly.

Для решения следует решать методом сопряженных градиентов. Это простой и понятный метод по сравнению с другими методами. Этот метод хорошо распараллеливается.

Программа должна в качестве входных данных должна получать $N$ - размер матрицы. В качестве результата работы программа должна выдать время работы.


# Серверная часть

Нужно написать серверную часть для управления параллельным решением СЛАУ на множестве решателей на WebAssembly.

Серверная часть должна поддерживать работу решателей через WebSocket. Это позволит улучшить отзывчивость взаимодействие с решателями.

Серверная часть должна распределить строки матрицы СЛАУ A между решателями.

Серверная часть должна осуществлять сборку скалярного произведения из слагаемых, а затем рассылать результат всем решателями.

Серверная часть может быть написана на любом серверном языке программирования (Node.js, Python, PHP, Java и т.п.).


# Решатель (клиентская часть)

Нужно написать решатель для выполнения вычислений.

Решатель должен иметь вычислительный модуль написанный на WebAssembly.

Решатель должен иметь модуль на Javascript для взаимодействия с серверной частью.

Решатель должен уметь скачивать и сохранять файлы в облачном сервисе хранения данных AWS S3 или его аналоге.

Решатель должен хранить:
- несколько строк матрицы СЛАУ: A
- части векторов: r_k, x_k
- векторы: b, p_k
- числа (скаляры): ɑ, β


# Диаграмма взаимодействия

Решатель должен отправлять на сервер:

Решатель должен получить от сервера

Решатель должен сохранять временной профиль - количество времени потраченного на каждом этапе вычислений, а также количество времени потраченного на передачи данных.


# Контрольная задача

Для оценки эффективности необходимо решить систему линейных алгебраических уравнений (СЛАУ).

Чтобы СЛАУ гарантированно имела решение, ее матрица должна быть симметричной и положительно определенной. Чтобы получить такую матрицу можно использовать следующий прием: $А = (B + B') / 2$, где $B$ - случайная квадратная матрица, $B'$ - транспонированная (перевернутая относительно главной диагонали) матрица.


# Что нужно продумать потом

Как хранить данные в AWS S3 в сжиматом виде?

Можно ли запускать код на GPU?

Как обеспечить надежность вычислений при отключении решателей?

Как обеспечить масштабирование при подключении новых решателей?


# Литература

[Метод сопряженных градиентов](https://en.wikipedia.org/wiki/Conjugate_gradient_method#The_resulting_algorithm)

[Документация AWS S3](https://docs.aws.amazon.com/s3/index.html)
