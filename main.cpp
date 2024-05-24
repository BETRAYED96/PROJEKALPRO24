#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

// Struct untuk menyimpan informasi ujian
struct Ujian {
    string nama;
    string soalFileName;
};

// Deklarasi awal fungsi
void displayClosingMessage();
void tambahUjian();
void lihatUjian();
void hapusUjian();
void signUp();
bool login();
string loginPengerjaSoal();
void displayAdminMenu();
void displayPengerjaSoalMenu();
void mulaiUjian(const string& username);
bool sudahMengerjakanUjianSebelumnya(const string& username);
void simpanSkor(const string& username, int skor);

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
int main() {
    int choice;
    bool loggedIn = false; // Status login
    bool loggedInAdmin = false; // Status login admin
    string username; // Username untuk pengerja soal

    do {
        cout << "=================================" << endl;
        cout << "         Sistem Ujian Online     " << endl;
        cout << "=================================" << endl;
        cout << "1. Login sebagai Pembuat Soal" << endl;
        cout << "2. Login sebagai Pengerja Soal" << endl;
        cout << "3. Sign up" << endl;
        cout << "4. Keluar program" << endl;
        cout << "Masukkan pilihan: ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Login sebagai Pembuat Soal
                if (login()) {
                    loggedIn = true;
                    loggedInAdmin = true;
                    system("cls");
                    cout << "Selamat datang, Admin!" << endl;
                    while (loggedInAdmin) {
                        displayAdminMenu();
                        cin >> choice;
                        switch (choice) {
                            case 1:
                                system("cls");
                                tambahUjian();
                                break;
                            case 2:
                                system("cls");
                                lihatUjian();
                                break;
                            case 3:
                                system("cls");
                                hapusUjian();
                                break;
                            case 4:
                                loggedInAdmin = false;
                                break;
                            default:
                                cout << "Pilihan tidak valid!" << endl;
                                system("pause");
                                system("cls");
                        }
                    }
                }
                break;
            case 2:
                // Login sebagai Pengerja Soal
                username = loginPengerjaSoal();
                if (!username.empty()) {
                    loggedIn = true;
                    system("cls");
                    cout << "Selamat datang, " << username << "!" << endl;
                    while (loggedIn) {
                        displayPengerjaSoalMenu();
                        cin >> choice;
                        switch (choice) {
                            case 1:
                                if (!sudahMengerjakanUjianSebelumnya(username)) {
                                    mulaiUjian(username);
                                } else {
                                    cout << "Anda sudah menyelesaikan ujian sebelumnya." << endl;
                                }
                                break;
                            case 2:
                                loggedIn = false;
                                break;
                            default:
                                cout << "Pilihan tidak valid!" << endl;
                                system("pause");
                                system("cls");
                        }
                    }
                }
                break;
            case 3:
                // Sign up
                system("cls");
                signUp();
                break;
            case 4:
                // Keluar program
                displayClosingMessage();
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
                system("pause");
                system("cls");
        }
    } while (choice != 4);

    return 0;
}

// Fungsi untuk menampilkan menu admin
void displayAdminMenu() {
    cout << "=================================" << endl;
    cout << "          Menu Admin             " << endl;
    cout << "=================================" << endl;
    cout << "1. Menambahkan ujian" << endl;
    cout << "2. Melihat ujian" << endl;
    cout << "3. Menghapus ujian" << endl;
    cout << "4. Keluar " << endl;
    cout << "Masukkan pilihan: ";
}

// Fungsi untuk menampilkan pesan penutup
void displayClosingMessage() {
    string closingMessages[] = {"Sampai Jumpa!", "Selamat Tinggal!", "Hati-hati di jalan!", "Semoga Hari Anda Menyenangkan!"};
    srand(time(0));
    int idx = rand() % (sizeof(closingMessages) / sizeof(closingMessages[0]));
    cout << closingMessages[idx] << endl;
}

// Fungsi untuk menambahkan ujian
void tambahUjian() {
    string namaUjian;
    cout << "Masukkan nama ujian: ";
    cin.ignore();
    getline(cin, namaUjian);

    // Simpan nama ujian ke dalam array
    // Dan simpan juga nama file soal
    Ujian ujian;
    ujian.nama = namaUjian;
    ujian.soalFileName = namaUjian + "_soal.txt";

    // Tulis ke file ujian.txt
    ofstream ujianFile("ujian.txt", ios::app);
    if (ujianFile.is_open()) {
        ujianFile << ujian.nama << endl;
        ujianFile.close();
    } else {
        cout << "Gagal membuka file ujian.txt" << endl;
        return;
    }

    // Membuat file soal.txt untuk ujian ini
    ofstream soalFile(ujian.soalFileName.c_str());
    if (!soalFile.is_open()) {
        cout << "Gagal membuat file soal ujian" << endl;
        return;
    }

    int numQuestions;
    cout << "Masukkan jumlah soal: ";
    cin >> numQuestions;
    cin.ignore();

    for (int i = 1; i <= numQuestions; ++i) {
        string pertanyaan, jawabanBenar;
        vector<string> pilihan;
        cout << "Masukkan pertanyaan " << i << ": ";
        getline(cin, pertanyaan);

        cout << "Masukkan jawaban benar (a/b/c/d): ";
        cin >> jawabanBenar;

        cout << "Masukkan pilihan jawaban (pisahkan dengan koma): ";
        cin.ignore();
        string pilihanJawaban;
        getline(cin, pilihanJawaban);

        // Memisahkan pilihan jawaban
        stringstream ss(pilihanJawaban);
        string pilihanItem;
        while (getline(ss, pilihanItem, ',')) {
            pilihan.push_back(pilihanItem);
        }

        // Tulis soal ke dalam file soal.txt
        soalFile << "Pertanyaan " << i << ": " << pertanyaan << endl;
        soalFile << "Jawaban Benar: " << jawabanBenar << endl;
        soalFile << "Pilihan Jawaban: ";
        for (size_t j = 0; j < pilihan.size(); ++j) {
            if (j > 0) {
                soalFile << ", ";
            }
            soalFile << pilihan[j];
        }
        soalFile << endl << endl;
    }

    soalFile.close();

    cout << "Ujian '" << ujian.nama << "' berhasil ditambahkan." << endl;
}
// Fungsi untuk memeriksa apakah pengguna sudah pernah mengerjakan ujian sebelumnya
bool sudahMengerjakanUjianSebelumnya(const string& username) {
    ifstream skorFile("skor.txt");
    string line;
    while (getline(skorFile, line)) {
        if (line.find(username) != string::npos) {
            return true;
        }
    }
    return false;
}

// Fungsi untuk melihat daftar ujian
void lihatUjian() {
    ifstream ujianFile("ujian.txt");
    if (!ujianFile.is_open()) {
        cout << "Belum ada ujian yang tersedia." << endl;
        return;
    }

    string ujian;
    system("cls");
    cout << "Daftar ujian:" << endl;
    while (getline(ujianFile, ujian)) {
        cout << "- " << ujian << endl;
    }
    ujianFile.close();
    system("pause");
    system("cls");
}

// Fungsi untuk menghapus ujian
void hapusUjian() {
    string namaUjian;
    cout << "Masukkan nama ujian yang akan dihapus: ";
    cin.ignore();
    getline(cin, namaUjian);

    string ujianFileName = "ujian.txt";
    ifstream ujianFile(ujianFileName.c_str());
    if (!ujianFile.is_open()) {
        cout << "Belum ada ujian yang tersedia." << endl;
        return;
    }

    string line;
    vector<string> ujianNames;
    while (getline(ujianFile, line)) {
        if (line != namaUjian) {
            ujianNames.push_back(line);
        }
    }
    ujianFile.close();

    ofstream newUjianFile(ujianFileName.c_str());
    if (!newUjianFile.is_open()) {
        cout << "Gagal membuka file ujian.txt" << endl;
        return;
    }
    for (string ujian : ujianNames) {
        newUjianFile << ujian << endl;
    }
    newUjianFile.close();

    string soalFileName = namaUjian + "_soal.txt";
    if (remove(soalFileName.c_str()) != 0) {
        cout << "Gagal menghapus file soal ujian" << endl;
        return;
    }

    cout << "Ujian '" << namaUjian << "' berhasil dihapus." << endl;
}

// Fungsi untuk memulai ujian
// Fungsi untuk memulai ujian
void mulaiUjian(const string& username) {
    // Membaca daftar ujian dari file ujian.txt
    ifstream ujianFile("ujian.txt");
    if (!ujianFile.is_open()) {
        cout << "Belum ada ujian yang tersedia." << endl;
        return;
    }

    vector<string> daftarUjian;
    string namaUjian;
    unsigned int nomorUjian = 1;
    while (getline(ujianFile, namaUjian)) {
        daftarUjian.push_back(namaUjian);
        cout << nomorUjian << ". " << namaUjian << endl;
        nomorUjian++;
    }
    ujianFile.close();

    // Meminta pengguna untuk memilih ujian
    unsigned int pilihanUjian;
    cout << "Pilih ujian yang ingin Anda kerjakan: ";
    cin >> pilihanUjian;

    // Memeriksa apakah pengguna sudah mengerjakan ujian sebelumnya
    if (sudahMengerjakanUjianSebelumnya(username)) {
        cout << "Anda sudah menyelesaikan ujian sebelumnya." << endl;
        return;
    }

    // Memulai ujian
    if (pilihanUjian >= 1 && pilihanUjian <= daftarUjian.size()) {
        string namaUjianTerpilih = daftarUjian[pilihanUjian - 1];
        string namaFileSoal = namaUjianTerpilih + "_soal.txt";

        ifstream soalFile(namaFileSoal);
        if (!soalFile.is_open()) {
            cout << "Gagal membuka file soal ujian." << endl;
            return;
        }

        // Logika ujian
        int skor = 0;
        string pertanyaan, tipeSoal, pilihanJawaban, jawabanBenar;
        unsigned int nomorSoal = 1;
        time_t start = time(nullptr);
        time_t waktuUjian = 50 * 60; // 50 menit dalam detik

        while (getline(soalFile, pertanyaan) && difftime(time(nullptr), start) < waktuUjian) {
            cout << "Pertanyaan " << nomorSoal << ": " << pertanyaan << endl;
            getline(soalFile, tipeSoal);
            getline(soalFile, pilihanJawaban);
            getline(soalFile, jawabanBenar);

            if (tipeSoal == "pg") {
                cout << "a. ";
                size_t pos = pilihanJawaban.find(",");
                cout << pilihanJawaban.substr(0, pos) << endl;
                pilihanJawaban.erase(0, pos + 1);

                cout << "b. ";
                pos = pilihanJawaban.find(",");
                cout << pilihanJawaban.substr(0, pos) << endl;
                pilihanJawaban.erase(0, pos + 1);

                cout << "c. ";
                pos = pilihanJawaban.find(",");
                cout << pilihanJawaban.substr(0, pos) << endl;
                pilihanJawaban.erase(0, pos + 1);

                cout << "d. " << pilihanJawaban << endl;

                cout << "Jawaban Anda (a/b/c/d): ";
            } else {
                cout << "Jawaban Anda: ";
            }

            time_t startSoal = time(nullptr);
            string jawaban;
            cin >> jawaban;

            time_t waktuPengisian = difftime(time(nullptr), startSoal);
            if (waktuPengisian > waktuUjian - difftime(time(nullptr), start)) {
                cout << "Waktu habis untuk menjawab soal ini." << endl;
                jawaban = "";
            }

            if (jawaban == jawabanBenar) {
                skor += 3;
            } else if (jawaban != "") {
                skor -= 1;
            }

            nomorSoal++;
        }
        soalFile.close();

        // Menyimpan skor pengguna setelah ujian selesai
        simpanSkor(username, skor);

        // Menampilkan pesan setelah selesai mengerjakan ujian
        cout << "Ujian telah selesai. Skor Anda: " << skor << endl;

        // Menampilkan opsi untuk melihat jawaban atau kembali ke menu
        char opsi;
        cout << "Apakah Anda ingin melihat jawaban yang benar dan salah? (y/n): ";
        cin >> opsi;
        if (opsi == 'y' || opsi == 'Y') {
            // Implementasi untuk menampilkan jawaban yang benar dan salah
            cout << "Jawaban yang benar dan salah akan ditampilkan di sini." << endl;
        } else {
            cout << "Kembali ke menu Sistem Ujian Online." << endl;
        }
    } else {
        cout << "Pilihan ujian tidak valid." << endl;
    }
}

// Fungsi untuk menyimpan skor ke dalam file
void simpanSkor(const string& username, int skor) {
    ofstream skorFile("skor.txt", ios::app);
    if (!skorFile.is_open()) {
        cout << "Gagal membuka file skor." << endl;
        return;
    }

    skorFile << username << ": " << skor << endl;
    skorFile.close();
}

// Fungsi untuk signup
void signUp() {
    string username, password;
    system("cls");
    cout << "===================================" << endl;
    cout << "              Sign Up" << endl;
    cout << "===================================" << endl;
    cout << "Masukkan username baru: ";
    cin >> username;
    cout << "Masukkan password baru: ";
    cin >> password;

    // Simpan ke dalam file user.txt
    ofstream userFile("user.txt", ios::app);
    if (!userFile.is_open()) {
        cout << "Gagal membuka file user.txt" << endl;
        return;
    }

    userFile << username << " " << password << endl;
    userFile.close();

    cout << "Registrasi berhasil!" << endl;
    system("pause");
    system("cls");
}

// Fungsi untuk login
bool login() {
    string username, password;
    cout << "===================================" << endl;
    cout << "               Login" << endl;
    cout << "===================================" << endl;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;
    ifstream userFile("user.txt");
    if (!userFile.is_open()) {
        cout << "Gagal membuka file user.txt" << endl;
        return false;
    }

    string fileUsername, filePassword;
    while (userFile >> fileUsername >> filePassword) {
        if (username == fileUsername && password == filePassword) {
            userFile.close();
            cout << "Login berhasil!" << endl;
            system("pause");
            system("cls");
            return true;
        }
    }

    userFile.close();
    cout << "Username atau password salah." << endl;
    system("pause");
    system("cls");
    return false;
}

// Fungsi untuk login pengerja soal
string loginPengerjaSoal() {
    string username, password;
    system("cls");
    cout << "===================================" << endl;
    cout << "               Login" << endl;
    cout << "===================================" << endl;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;

    ifstream userFile("user.txt");
    if (!userFile.is_open()) {
        cout << "Gagal membuka file user.txt" << endl;
        return "";
    }

    string fileUsername, filePassword;
    while (userFile >> fileUsername >> filePassword) {
        if (username == fileUsername && password == filePassword) {
            userFile.close();
            cout << "Login berhasil!" << endl;
            return username;
        }
    }

    userFile.close();
    cout << "Username atau password salah." << endl;
    system("pause");
    system("cls");
    return "";
}

// Fungsi untuk menampilkan menu pengerja soal
void displayPengerjaSoalMenu() {
    cout << "=================================" << endl;
    cout << "       Menu Pengerja Soal        " << endl;
    cout << "=================================" << endl;
    cout << "1. Mulai ujian" << endl;
    cout << "2. Keluar " << endl;
    cout << "Masukkan pilihan: ";
}
