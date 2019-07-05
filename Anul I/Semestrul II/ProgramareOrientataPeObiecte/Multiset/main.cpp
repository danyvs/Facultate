#include <iostream>
#include "multiset.h"

using namespace std;

char toUpper(char c) {
    return c - 'a' + 'A';
}

int main() {
    {
        Multiset<int> M;
        int dim;
        cin >> dim;
        assert(dim > 0);
        for (int i = 0; i <= dim; ++i)
            M.append(i);
        M.append(0);
        M.removeFirst(0);
        M.removeFirst(10);
        M.append(2);
        M.append(2);
        cout << M.count(2) << "\n";
        M.append(1);
        assert(M.contains(1));
        cout << M.contains(10) << "\n";
        cout << M.size() << "\n";
        cout << M << "\n";

        cout << "\n";
        Multiset<int> N(M);
        N.removeAll(1);
        N.removeAll(2);
        N.removeFirst(3);
        cout << N << "\n";

        cout << "\n";
        N = M;
        cout << N << "\n";

        try {
            if (!N.size())
                throw -1;
        }
        catch (int e) {
            cout << "Multisetul este vid!\n";
        }
        Multiply I(10);
        N.transform(I);
        cout << N << "\n";
    }

    {
        int dim;
        Multiset<char> M1;
        cin >> dim;
        assert(dim > 0);
        for (int i = 0; i < dim; ++i) {
            bool done = false;
            char c;
            while (!done) {
                done = true;

                cin >> c;

                try {
                    if (c < 'a' || c > 'z') {
                        cout << "Trebuie sa introduceti o litera mica!\n";
                        throw -1;
                    }
                }
                catch (int e) {
                    done = false;
                }

            }
            M1.append(c);
        }

        cout << M1.size() << "\n";
        cout << M1 << "\n";

        Multiset<char> M2(M1);
        M2.transform(toUpper);
        cout << M2 << "\n";

        Multiset<char> M3;
        M3.append('z');
        cout << M3.contains('z') << "\n";
        cout << M3 << "\n";
        M3.removeFirst('z');
        cout << M3 << "\n";
        M3 = M1;
        cout << M1.contains('z') << "\n";
        M3.removeAll('i');
        cout << M3 << "\n";
    }

    return 0;
}
