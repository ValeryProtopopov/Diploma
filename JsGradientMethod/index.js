function innerProduct(U, V) // Скалярное произведение
{
    let result = 0;
    for (let i = 0; i < SIZE; i++) {
        result += U[i] * V[i];
    }
    return result;
}