//#include <Windows.h>
//#include <cmath>
//#include"cs2_data/client_dll.hpp"
//#include"cs2_data/offsets.hpp"
//#include"cs2_data/buttons.hpp"
//
//// Структура для зберігання тривимірного вектора
//struct Vector3 {
//    float x, y, z;
//};
//
//// Функція для обчислення кута наведення
//Vector3 CalculateAngle(const Vector3& localPosition, const Vector3& enemyPosition) {
//    Vector3 delta = { enemyPosition.x - localPosition.x, enemyPosition.y - localPosition.y, enemyPosition.z - localPosition.z };
//    float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
//    Vector3 angles;
//    angles.x = atan(delta.z / hyp) * (180.0f / 3.14159265f); // Підйом
//    angles.y = atan(delta.y / delta.x) * (180.0f / 3.14159265f); // Азимут
//    if (delta.x >= 0.0f) angles.y += 180.0f;
//    return angles;
//}
//
//// Функція для читання пам'яті процесу (для отримання даних гри)
//template<typename T>
//T ReadMemory(HANDLE process, DWORD address) {
//    T buffer;
//    ReadProcessMemory(process, (LPVOID)address, &buffer, sizeof(T), NULL);
//    return buffer;
//}
//
//// Основна функція, яка буде виконувати наведення
//void AimBot(HANDLE processHandle, DWORD localPlayerBase, DWORD enemyBase) {
//    Vector3 localPosition = ReadMemory<Vector3>(processHandle, localPlayerBase + 0x134); // Замість 0x134 підставте правильний офсет
//    Vector3 enemyPosition = ReadMemory<Vector3>(processHandle, enemyBase + 0x134);
//
//    Vector3 aimAngle = CalculateAngle(localPosition, enemyPosition);
//
//    // Тут потрібно встановити кут прицілу, але це специфічно для кожної гри
//}
//
////int main() {
////    // Отримайте HANDLE процесу гри, знайдіть базові адреси, і викликайте AimBot в циклі або на тригер
////    return 0;
////}
