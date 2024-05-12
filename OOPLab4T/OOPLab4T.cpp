#include <iostream>
#include <cmath>
using namespace std;
class Vector3F {
private:
    float x, y, z;
    int state; // 0 - нормальний стан, 1 - помилка

public:
    static int objectCount;

    Vector3F() : x(0), y(0), z(0), state(0) {
        objectCount++;
    }

    Vector3F(float value) : x(value), y(value), z(value), state(0) {
        objectCount++;
    }

    Vector3F(float* arr) : state(0) {
        if (arr == nullptr) {
            state = 1;
        }
        else {
            x = arr[0];
            y = arr[1];
            z = arr[2];
        }
        objectCount++;
    }

    ~Vector3F() {
        cout << "Vector state: " << state << endl;
        objectCount--;
    }

    Vector3F& operator++() {
        x++;
        y++;
        z++;
        return *this;
    }

    Vector3F operator++(int) {
        Vector3F temp = *this;
        ++(*this);
        return temp;
    }

    Vector3F& operator--() {
        x--;
        y--;
        z--;
        return *this;
    }

    Vector3F operator--(int) {
        Vector3F temp = *this;
        --(*this);
        return temp;
    }

    bool operator!() {
        return !(x == 0 && y == 0 && z == 0);
    }

    Vector3F& operator=(const Vector3F& other) {
        if (this == &other)
            return *this;
        x = other.x;
        y = other.y;
        z = other.z;
        state = other.state;
        return *this;
    }

    Vector3F& operator+=(const Vector3F& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3F& operator-=(const Vector3F& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3F& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3F& operator/=(float scalar) {
        if (scalar == 0) {
            state = 1;
        }
        else {
            x /= scalar;
            y /= scalar;
            z /= scalar;
        }
        return *this;
    }


    Vector3F operator%=(int divisor) {
        if (divisor == 0) {
            state = 1;
            return *this;
        }
        x = fmod(x, divisor);
        y = fmod(y, divisor);
        z = fmod(z, divisor);
        return *this;
    }

    friend Vector3F operator+(Vector3F lhs, const Vector3F& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Vector3F operator-(Vector3F lhs, const Vector3F& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Vector3F operator*(Vector3F vec, float scalar) {
        vec *= scalar;
        return vec;
    }

    friend Vector3F operator*(float scalar, Vector3F vec) {
        vec *= scalar;
        return vec;
    }

    friend Vector3F operator/(Vector3F vec, float scalar) {
        vec /= scalar;
        return vec;
    }

    friend Vector3F operator%(Vector3F vec, int divisor) {
        vec %= divisor;
        return vec;
    }

    friend bool operator==(const Vector3F& lhs, const Vector3F& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    friend bool operator!=(const Vector3F& lhs, const Vector3F& rhs) {
        return !(lhs == rhs);
    }

    float& operator[](int index) {
        if (index < 0 || index > 2) {
            state = 1;
            return z;
        }
        if (index == 0) return x;
        else if (index == 1) return y;
        else return z;
    }

    void* operator new(size_t size) {
        cout << "Allocating memory for Vector3F" << endl;
        return ::operator new(size);
    }

    void operator delete(void* ptr) {
        cout << "Deallocating memory for Vector3F" << endl;
        ::operator delete(ptr);
    }

    friend ostream& operator<<(ostream& os, const Vector3F& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    friend istream& operator>>(istream& is, Vector3F& vec) {
        is >> vec.x >> vec.y >> vec.z;
        return is;
    }

    bool allNonZero() {
        return x != 0 && y != 0 && z != 0;
    }

    bool allZero() {
        return x == 0 && y == 0 && z == 0;
    }

    bool allPositive() {
        return x > 0 && y > 0 && z > 0;
    }

    bool allNegative() {
        return x < 0 && y < 0 && z < 0;
    }

    bool allSameSign() {
        return (x >= 0 && y >= 0 && z >= 0) || (x <= 0 && y <= 0 && z <= 0);
    }
};

int Vector3F::objectCount = 0;


class BusSchedule {
    struct Node {
        string busNumber;
        string departureTime;
        Node* next;

        Node(const string& number, const string& time) : busNumber(number), departureTime(time), next(nullptr) {}
    };

    Node* head;
    int errorCode;

public:
    enum ErrorCodes {
        NO_ERROR = 0,
        BUS_NUMBER_NOT_FOUND
    };

    BusSchedule() : head(nullptr), errorCode(NO_ERROR) {}

    ~BusSchedule() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addSchedule(const string& busNumber, const string& departureTime) {
        Node* newNode = new Node(busNumber, departureTime);
        newNode->next = head;
        head = newNode;
    }

    string operator[](const string& busNumber) {
        Node* current = head;
        while (current != nullptr) {
            if (current->busNumber == busNumber) {
                errorCode = NO_ERROR;
                return current->departureTime;
            }
            current = current->next;
        }
        errorCode = BUS_NUMBER_NOT_FOUND;
        return "";
    }

    string operator()(const string& busNumber) {
        return (*this)[busNumber];
    }

    int getErrorCode() const {
        return errorCode;
    }

    friend ostream& operator<<(ostream& os, const BusSchedule& schedule) {
        Node* current = schedule.head;
        while (current != nullptr) {
            os << "Bus Number: " << current->busNumber << ", Departure Time: " << current->departureTime << endl;
            current = current->next;
        }
        return os;
    }

    friend istream& operator>>(istream& is, BusSchedule& schedule) {
        string busNumber, departureTime;
        while (is >> busNumber >> departureTime) {
            schedule.addSchedule(busNumber, departureTime);
        }
        return is;
    }
};



int main() {
    int task;
    cout << "Enter task";
    cin >> task;
    switch (task)
    {
    case 1: {
        Vector3F vec1;
        cout << "vec1: " << vec1 << endl;

        Vector3F vec2(1.1f);
        cout << "vec2: " << vec2 << endl;

        float arr[] = { 2.2f, 3.3f, 4.4f };
        Vector3F vec3(arr);
        cout << "vec3: " << vec3 << endl;

        vec1++;
        cout << "vec1 after ++: " << vec1 << endl;

        vec1--;
        cout << "vec1 after --: " << vec1 << endl;

        cout << "Logical NOT of vec1: " << !vec1 << endl;

        Vector3F vec5;
        vec5 = vec3;
        cout << "vec5: " << vec5 << endl;

        vec1 += vec2;
        cout << "vec1 += vec2: " << vec1 << endl;

        vec1 -= vec2;
        cout << "vec1 -= vec2: " << vec1 << endl;

        vec1 *= 2;
        cout << "vec1 *= 2: " << vec1 << endl;

        vec1 /= 2;
        cout << "vec1 /= 2: " << vec1 << endl;

        cout << "vec2 + vec3: " << vec2 + vec3 << endl;
        cout << "vec3 - vec2: " << vec3 - vec2 << endl;
        cout << "vec3 * 2: " << vec3 * 2 << endl;
        cout << "vec3 / 2: " << vec3 / 2 << endl;
        cout << "vec3 % 2: " << vec3 % 2 << endl;

        cout << "vec3[0]: " << vec3[0] << endl;
        cout << "vec3[1]: " << vec3[1] << endl;
        cout << "vec3[2]: " << vec3[2] << endl;
        cout << "vec3[3]: " << vec3[3] << endl;
        break;
    }
    case 2: {
        BusSchedule schedule;

        schedule.addSchedule("101", "10:00");
        schedule.addSchedule("102", "11:00");

        cout << "Bus Schedule:" << endl;
        cout << schedule << endl;

        string busNumberToFind = "102";
        string departureTime = schedule[busNumberToFind];
        if (schedule.getErrorCode() == BusSchedule::NO_ERROR) {
            cout << "Departure time for bus number " << busNumberToFind << ": " << departureTime << endl;
        }
        else {
            cout << "Bus number " << busNumberToFind << " not found!" << endl;
        }

        string departureTime2 = schedule(busNumberToFind);
        if (!departureTime2.empty()) {
            cout << "Departure time for bus number " << busNumberToFind << ": " << departureTime2 << endl;
        }
        else {
            cout << "Bus number " << busNumberToFind << " not found!" << endl;
        }
        break;
    }
    }
}


