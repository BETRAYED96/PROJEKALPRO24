#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// Struktur untuk menyimpan soal
struct Soal {
    string tipe;
    string pertanyaan;
    vector<string> pilihanGanda;
    string jawabanBenar;
};

vector<Soal> bankSoal;
vector<string> bankNamaUjian;
vector<vector<Soal>> ujianSoal;
string usernamePembuat = "admin";
string passwordPembuat = "admin123";

// Function to save user data to a file
void saveUserData(const string& nama, const string& password) {
    ofstream outFile("database.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "USER " << nama << " " << password << endl;
        outFile.close();
    } else {
        cerr << "Unable to open file for writing user data." << endl;
    }
}

// Function to save soal data to a file
void saveSoalData(const Soal& soal, int ujianIndex) {
    ofstream outFile("database.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "SOAL " << soal.tipe << " " << soal.pertanyaan << endl;
        if (soal.tipe == "pg") {
            for (const auto& pilihan : soal.pilihanGanda) {
                outFile << "OPTION " << pilihan << endl;
            }
        }
        outFile << "ANSWER " << soal.jawabanBenar << endl;
        outFile << "UJIANINDEX " << ujianIndex << endl;
        outFile.close();
    } else {
        cerr << "Unable to open file for writing soal data." << endl;
    }
}

// Function to save ujian data to a file
void saveUjianData(const string& namaUjian) {
    ofstream outFile("database.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "UJIAN " << namaUjian << endl;
        outFile.close();
    } else {
        cerr << "Unable to open file for writing ujian data." << endl;
    }
}

// Function to load data from the file
void loadData() {
    ifstream inFile("database.txt");
    if (inFile.is_open()) {
        string type;
        while (inFile >> type) {
            if (type == "USER") {
                string nama, password;
                inFile >> nama >> password;
                // User data can be handled here if needed
            } else if (type == "SOAL") {
                Soal soal;
                int ujianIndex;
                inFile >> soal.tipe;
                inFile.ignore();
                getline(inFile, soal.pertanyaan);
                if (soal.tipe == "pg") {
                    string option;
                    for (int i = 0; i < 4; ++i) {
                        inFile >> type; // Read "OPTION"
                        inFile.ignore();
                        getline(inFile, option);
                        soal.pilihanGanda.push_back(option);
                    }
                }
                inFile >> type; // Read "ANSWER"
                inFile.ignore();
                getline(inFile, soal.jawabanBenar);
                inFile >> type; // Read "UJIANINDEX"
                inFile >> ujianIndex;
                while (ujianIndex >= ujianSoal.size()) {
                    ujianSoal.push_back({});
                }
                ujianSoal[ujianIndex].push_back(soal);
            } else if (type == "UJIAN") {
                string namaUjian;
                inFile.ignore();
                getline(inFile, namaUjian);
                bankNamaUjian.push_back(namaUjian);
            }
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading data." << endl;
    }
}

bool login(string nama, string password, string role) {
    if (role == "pembuat" && nama == usernamePembuat && password == passwordPembuat) {
        return true;
    } else if (role == "pengerja") {
        ifstream inFile("database.txt");
        if (inFile.is_open()) {
            string type, storedName, storedPassword;
            while (inFile >> type >> storedName >> storedPassword) {
                if (type == "USER" && storedName == nama && storedPassword == password) {
                    inFile.close();
                    return true;
                }
            }
            inFile.close();
        }
    }
    return false;
}

void buatSoal(int ujianIndex) {
    string tipeSoal;
    cout << "Masukkan tipe soal (pg/essay): ";
    cin >> tipeSoal;
    Soal soal;
    soal.tipe = tipeSoal;
    cout << "Masukkan pertanyaan: ";
    cin.ignore();
    getline(cin, soal.pertanyaan);

    if (tipeSoal == "pg") {
        string pilihan;
        for (int i = 0; i < 4; ++i) {
            cout << "Masukkan pilihan ke-" << i+1 << ": ";
            getline(cin, pilihan);
            soal.pilihanGanda.push_back(pilihan);
        }
    }

    cout << "Masukkan jawaban benar: ";
    getline(cin, soal.jawabanBenar);

    while (ujianIndex >= ujianSoal.size()) {
        ujianSoal.push_back({});
    }
    ujianSoal[ujianIndex].push_back(soal);
    saveSoalData(soal, ujianIndex);
    cout << "Soal berhasil dibuat!\n";
}

void buatUjian() {
    string namaUjian;
    cout << "Masukkan nama ujian: ";
    cin >> namaUjian;
    bankNamaUjian.push_back(namaUjian);
    saveUjianData(namaUjian);

    int ujianIndex = bankNamaUjian.size() - 1;
    char lagi;
    do {
        buatSoal(ujianIndex);
        cout << "Apakah ingin membuat soal lagi? (y/t): ";
        cin >> lagi;
    } while (lagi == 'y');

    cout << "Terima kasih telah membuat soal!\n";
}

void lihatUjian() {
    cout << "Daftar ujian:\n";
    for (int i = 0; i < bankNamaUjian.size(); ++i) {
        cout << i+1 << ". " << bankNamaUjian[i] << "\n";
    }
}

void pengerjaSoal() {
    string nama, password;
    char sudahPunyaAkun;
    cout << "Apakah sudah mempunyai akun? (y/t): ";
    cin >> sudahPunyaAkun;

    if (sudahPunyaAkun == 't') {
        system("cls");
        cout << "Sign Up - Masukkan Nama: ";
        cin >> nama;
        cout << "Masukkan Password: ";
        cin >> password;
        saveUserData(nama, password);
    }

    cout << "Login - Masukkan Nama: ";
    cin >> nama;
    cout << "Masukkan Password: ";
    cin >> password;

    if (login(nama, password, "pengerja")) {
        cout << "Login berhasil!\n" << endl;
        system("pause");
        system("cls");
        int pilihanUjian;
        lihatUjian();
        cout << "Pilih nomor ujian yang ingin dikerjakan: ";
        cin >> pilihanUjian;

        if (pilihanUjian > 0 && pilihanUjian <= bankNamaUjian.size()) {
            int ujianIndex = pilihanUjian - 1;
            int skor = 0;
            for (const auto& soal : ujianSoal[ujianIndex]) {
                cout << soal.pertanyaan << "\n";
                if (soal.tipe == "pg") {
                    for (int i = 0; i < soal.pilihanGanda.size(); ++i) {
                        cout << char('a' + i) << ". " << soal.pilihanGanda[i] << "\n";
                    }
                    char jawaban;
                    cin >> jawaban;
                    if (tolower(jawaban) == soal.jawabanBenar[0]) {
                        skor += 3;
                    } else {
                        skor -= 1;
                    }
                } else if (soal.tipe == "essay") {
                    string jawaban;
                    cin.ignore();
                    getline(cin, jawaban);
                    if (jawaban == soal.jawabanBenar) {
                        skor += 3;
                    } else {
                        skor -= 1;
                    }
                }
            }
            cout << "Ujian selesai! Skor Anda: " << skor << "\n";
        } else {
            cout << "Ujian tidak valid.\n";
        }
    } else {
        cout << "Login gagal.\n";
    }
}

int main() {
    loadData(); // Load data from file at the start of the program

    int pilihan;
    cout << "=================================" << endl;
    cout << "      Sistem Ujian Online" << endl;
    cout << "=================================" << endl;
    cout << "1. Login sebagai Pembuat Soal\n";
    cout << "2. Login sebagai Pengerja Soal\n";
    cout << "Masukkan pilihan: ";
    cin >> pilihan;

    if (pilihan == 1) {
        system("cls");
        string nama, password;
        cout << "Masukkan Nama: ";
        cin >> nama;
        cout << "Masukkan Password: ";
        cin >> password;

        if (login(nama, password, "pembuat")) {
            int pilihanMenu;
            do {
                system("cls");
                cout << "Menu Pembuat Soal\n";
                cout << "1. Membuat Ujian Baru\n";
                cout << "2. Melihat Ujian\n";
                cout << "3. Keluar\n";
                cout << "Masukkan pilihan: ";
                cin >> pilihanMenu;

                switch (pilihanMenu) {
                    case 1:
                        buatUjian();
                        break;
                    case 2:
                        lihatUjian();
                        break;
                    case 3:
                        cout << "Keluar dari sistem.\n";
                        break;
                    default:
                        cout << "Pilihan tidak valid.\n";
                }
            } while (pilihanMenu != 3);
        } else {
            cout << "Login gagal.\n";
        }
    } else if (pilihan == 2) {
        pengerjaSoal();
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    return 0;
}
