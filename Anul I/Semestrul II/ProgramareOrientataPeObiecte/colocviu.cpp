// Sava Vasile-Daniel, 143, MinGW, CLion

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

class Bilet {
    string statiePlecare_;
    string statieSosire_;
    int data_, ora_;
    int durata_;
protected:
    string clasa_;
    const double taxaConfort = 1.2;
    int distanta_;
    static int nr_;

    string codTren_;
    double pret_;
    string serie_;
public:
    Bilet();
    Bilet(const string &plecare, const string &sosire, const int &data, const int &ora, const int &durata, const int &distanta, const int &clasa);
    virtual ~Bilet() = 0;
    Bilet(const Bilet &bilet);
    Bilet &operator = (const Bilet &bilet);

    friend istream &operator >> (istream &in, Bilet &bilet);
    friend ostream &operator << (ostream &out, const Bilet &bilet);

    string getCodTren() const;
    int getDistanta() const;
    string getSerie() const;
};

Bilet::Bilet() {
    data_ = ora_ = distanta_ = durata_ = 0;
    pret_ = 0;
    clasa_ = '0';
    ++nr_;
}

Bilet::~Bilet() = default;

Bilet::Bilet(const string &plecare, const string &sosire, const int &data, const int &ora, const int &durata, const int &distanta, const int &clasa) {
    statiePlecare_ = plecare;
    statieSosire_ = sosire;
    data_ = data;
    ora_ = ora;
    durata_ = durata;
    distanta_ = distanta;
    clasa_ = clasa;
}

Bilet::Bilet(const Bilet &bilet) {
    statiePlecare_ = bilet.statiePlecare_;
    statieSosire_ = bilet.statieSosire_;
    data_ = bilet.data_;
    ora_ = bilet.ora_;
    durata_ = bilet.durata_;
    distanta_ = bilet.distanta_;
    clasa_ = bilet.clasa_;
    pret_ = bilet.pret_;
}

Bilet &Bilet::operator = (const Bilet &bilet) {
    if (&bilet != this) {
        statiePlecare_ = bilet.statiePlecare_;
        statieSosire_ = bilet.statieSosire_;
        data_ = bilet.data_;
        ora_ = bilet.ora_;
        durata_ = bilet.durata_;
        distanta_ = bilet.distanta_;
        clasa_ = bilet.clasa_;
        pret_ = bilet.pret_;
    }
    return *this;
}

istream &operator >> (istream &in, Bilet &bilet) {
    in >> bilet.statiePlecare_ >> bilet.statieSosire_;
    in >> bilet.data_ >> bilet.ora_;
    in >> bilet.codTren_ >> bilet.clasa_;
    in >> bilet.durata_ >> bilet.distanta_;
    assert(bilet.data_ >= 1 && bilet.data_ <= 31);
    assert(bilet.ora_ >= 0 && bilet.ora_ <= 23);
    assert(bilet.clasa_ == "I" || bilet.clasa_ == "II");
    return in;
}

ostream &operator << (ostream &out, const Bilet &bilet) {
    out << bilet.statiePlecare_ << " - " << bilet.statieSosire_ << "\n";
    out << bilet.data_ << " " << bilet.ora_ << "\n";
    out << bilet.durata_ << " " << bilet.distanta_ << "\n";
    out << bilet.codTren_ << bilet.clasa_ << "\n";
    return out;
}

string Bilet::getCodTren() const {
    return codTren_;
}

int Bilet::getDistanta() const {
    return distanta_;
}

string Bilet::getSerie() const {
    return serie_;
}



class BiletRegio : public Bilet {
    const double pretKilometru_ = 0.39;
public:
    BiletRegio();
    ~BiletRegio() override;

    friend ostream &operator << (ostream &out, const BiletRegio &bilet);
};

BiletRegio::BiletRegio() {
    pret_ = pretKilometru_ * distanta_;
    if (clasa_ == "I")
        pret_ = taxaConfort * pret_;

    serie_ += "R";
    serie_ += codTren_;
    serie_ += clasa_;
    serie_.push_back('-');
    // serie_ += string(nr_);
}

BiletRegio::~BiletRegio() = default;

ostream &operator << (ostream &out, const BiletRegio &bilet) {
    out << "Regio ";
    out << static_cast<const Bilet&>(bilet);
    return out;
}



class BiletInterregio : public Bilet {
    const double pretKilometru_ = 0.7;
public:
    BiletInterregio();
    ~BiletInterregio() override;

    friend ostream &operator << (ostream &out, const BiletInterregio &bilet);
};

BiletInterregio::BiletInterregio() {
    pret_ = pretKilometru_ * distanta_;
    if (clasa_ == "I")
        pret_ = taxaConfort * pret_;

    serie_ += "IR";
    serie_ += codTren_;
    serie_ += clasa_;
    serie_.push_back('-');
    // serie_ += string(nr_);
}

BiletInterregio::~BiletInterregio() = default;

ostream &operator << (ostream &out, const BiletInterregio &bilet) {
    out << "Interregio ";
    out << static_cast<const Bilet&>(bilet);
    return out;
}



class ManagementBilete {
    vector<Bilet*> bilete_;
public:
    void adaugaBiletNou();
    void listareBileteTren(const string &cod);
    void listareBileteCalatorieLunga(const int &distantaMinima);
    void anulareBilet(const string &serie);
};

void ManagementBilete::adaugaBiletNou() {
    cout << "Bilet Regio/ Interregio: ";
    string tip;

    bool done = false;
    while (!done) {
        done = true;

        cin >> tip;

        try {
            if (tip != "Regio" && tip != "Interregio")
                throw -1;
        }
        catch (int e) {
            cout << "Nu a fost introdus un tip corect! Reintroduceti!\n";
            done = false;
        }
    }

    if (tip == "Regio") {
        BiletRegio bilet;
        cin >> bilet;
        bilete_.push_back(new BiletRegio(bilet));
    }
    else {
        BiletInterregio bilet;
        cin >> bilet;
        bilete_.push_back(new BiletInterregio(bilet));
    }
}

void ManagementBilete::listareBileteTren(const string &cod) {
    for (const auto &bilet : bilete_)
        if (bilet->getCodTren() == cod)
            cout << bilet << "\n";
}

void ManagementBilete::listareBileteCalatorieLunga(const int &distantaMinima) {
    for (const auto &bilet : bilete_)
        if (bilet->getDistanta() >= distantaMinima)
            cout << bilet << "\n";
}

void ManagementBilete::anulareBilet(const string &serie) {
    for (auto bilet = bilete_.begin(); bilet != bilete_.end(); ++bilet)
        if ((*bilet)->getSerie() == serie) {
            bilete_.erase(bilet);
            cout << "Biletul a fost anulat!\n";
            return;
        }
    cout << "Biletul nu a fost gasit!\n";
}

int Bilet::nr_ = 0;

int main() {
    ManagementBilete M;

    int n;
    cin >> n;
    assert(n > 0);
    for (int i = 0; i < n; ++i)
        M.adaugaBiletNou();

    M.listareBileteTren("123");
    M.listareBileteCalatorieLunga(200);
    M.anulareBilet("R123I-1");


    return 0;
}

