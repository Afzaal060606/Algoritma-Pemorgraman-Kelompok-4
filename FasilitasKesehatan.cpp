#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype> 
#include <conio.h> // Library KHUSUS untuk membuat password bintang (*) di Windows

using namespace std;

// --- KONFIGURASI DATA ---
const int MAX_WILAYAH = 10;
const int MAX_FASKES = 40;
const int MAX_USER = 5; 
const string EMAIL_DOMAIN = "@dinkes.jogjaprov.go.id"; 

// --- STRUKTUR DATA ---
struct Wilayah {
    string ID;
    string Nama;
    int JumlahPenduduk;
    int TotalFaskes;
    double RasioKesenjangan;
    int Peringkat;
};

struct Faskes {
    string ID;
    string Nama;
    string IDWilayah;
    string Tipe;
};

struct Pengguna {
    string NamaLengkap;
    string Email;
    string Jabatan;
    string Password;
};

// --- VARIABEL GLOBAL ---
Wilayah dataWilayah[MAX_WILAYAH];
Faskes dataFaskes[MAX_FASKES];
Pengguna dataUser[MAX_USER]; 

int n_wilayah = 0;
int n_faskes = 0;
int n_user = 0; 
int currentUserIndex = -1; 

// --- FUNGSI BANTUAN (HELPER) ---

// 1. Mengubah string ke huruf besar semua
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// 2. Memformat angka output menjadi format ribuan (cth: 350000 -> 350.000)
string FormatAngkaRibuan(int angka) {
    string s = to_string(angka);
    int n = s.length() - 3;
    while (n > 0) {
        s.insert(n, ".");
        n -= 3;
    }
    return s;
}

// 3. Menerima input angka dengan format titik (cth: 350.000 -> 350000)
int InputAngkaFormatTitik(string prompt) {
    string inputStr;
    int hasil = 0;
    bool valid = false;

    while (!valid) {
        cout << prompt;
        cin >> inputStr;

        string numbersOnly = "";
        bool isNumeric = true;

        for (char c : inputStr) {
            if (isdigit(c)) {
                numbersOnly += c;
            } else if (c == '.') {
                continue; 
            } else {
                isNumeric = false; 
                break;
            }
        }

        if (isNumeric && !numbersOnly.empty()) {
            try {
                hasil = stoi(numbersOnly);
                valid = true;
            } catch (...) {
                cout << "[ERROR] Angka terlalu besar!" << endl;
            }
        } else {
            cout << "[ERROR] Input salah! Masukkan angka saja." << endl;
        }
    }
    return hasil;
}

// 4. Fungsi Masking Password
string InputPasswordMasking() {
    string pass = "";
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) { // ENTER
            break;
        } else if (ch == 8) { // BACKSPACE
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        } else {
            pass += ch;
            cout << "*";
        }
    }
    return pass;
}

// 5. Validasi Domain Email
bool IsValidEmail(string email) {
    if (email.length() <= EMAIL_DOMAIN.length()) return false;
    string endOfEmail = email.substr(email.length() - EMAIL_DOMAIN.length());
    return (endOfEmail == EMAIL_DOMAIN);
}


// --- SISTEM AUTENTIKASI ---

void RegisterUser() {
    system("cls");
    cout << "\n=============================================" << endl;
    cout << "   REGISTRASI AKUN BARU DINKES DIY" << endl;
    cout << "=============================================" << endl;

    if (n_user >= MAX_USER) {
        cout << "[ERROR] Kuota pendaftaran penuh." << endl;
        cout << "Tekan Enter kembali ke menu awal...";
        cin.ignore(); cin.get();
        return;
    }

    Pengguna u;
    
    cout << "Nama Lengkap : ";
    cin.ignore();
    getline(cin, u.NamaLengkap);

    bool emailValid = false;
    while (!emailValid) {
        cout << "Email Dinas  : ";
        cin >> u.Email;
        
        if (IsValidEmail(u.Email)) {
            bool exists = false;
            for(int i=0; i<n_user; i++) {
                if(dataUser[i].Email == u.Email) exists = true;
            }
            if(exists) {
                cout << "[ERROR] Email sudah terdaftar! Gunakan email lain.\n";
            } else {
                emailValid = true;
            }
        } else {
            cout << "[ERROR] Email wajib menggunakan domain " << EMAIL_DOMAIN << "\n";
        }
    }

    cout << "Jabatan      : ";
    cin.ignore();
    getline(cin, u.Jabatan);

    cout << "Password     : ";
    u.Password = InputPasswordMasking();
    cout << endl;

    dataUser[n_user] = u;
    n_user++;

    cout << "\n[SUCCESS] Registrasi Berhasil! Silakan Login." << endl;
    cout << "Tekan Enter untuk ke Halaman Login...";
    cin.ignore(); cin.get(); 
}

bool LoginUser() {
    string inputEmail, inputPass;
    int percobaan = 0;

    while (percobaan < 3) {
        system("cls");
        cout << "\n=============================================" << endl;
        cout << "   LOGIN SISTEM DINKES PROVINSI DIY" << endl;
        cout << "=============================================" << endl;
        cout << "Note: Email wajib berakhiran " << EMAIL_DOMAIN << endl;
        cout << "---------------------------------------------" << endl;
        
        if (percobaan > 0) cout << "[WARNING] Email/Password Salah! Sisa: " << (3 - percobaan) << endl;

        cout << "Email    : ";
        cin >> inputEmail;

        cout << "Password : ";
        inputPass = InputPasswordMasking();
        cout << endl;

        // Jika user belum register sama sekali, tapi mencoba login
        if (n_user == 0) {
             cout << "\n[ERROR] Belum ada user terdaftar. Silakan Registrasi dulu." << endl;
             cout << "Tekan Enter...";
             cin.ignore(); cin.get();
             return false;
        }

        bool found = false;
        for (int i = 0; i < n_user; i++) {
            if (dataUser[i].Email == inputEmail && dataUser[i].Password == inputPass) {
                currentUserIndex = i; 
                found = true;
                break;
            }
        }

        if (found) {
            cout << "\n[SUCCESS] Login Berhasil!" << endl;
            cout << "Memuat Dashboard..." << endl;
            for(int k=0; k<50000000; k++); // Delay simulasi
            return true;
        } else {
            cout << "\n[ERROR] Akun tidak ditemukan atau password salah." << endl;
            cout << "Tekan Enter untuk mencoba lagi...";
            cin.ignore(); cin.get();
            percobaan++;
        }
    }
    
    cout << "\n[BLOCKED] Terlalu banyak percobaan gagal. Aplikasi keluar." << endl;
    return false;
}

void HalamanAwal() {
    int pilihan;
    bool running = true;

    while (running) {
        system("cls");
        cout << "=============================================" << endl;
        cout << "  PORTAL DATA KESEHATAN PROVINSI DIY" << endl;
        cout << "=============================================" << endl;
        cout << "1. Login Petugas" << endl;
        cout << "2. Registrasi Akun Baru" << endl;
        cout << "3. Keluar" << endl;
        cout << "---------------------------------------------" << endl;
        cout << "Pilihan (1-3): ";
        
        if (cin >> pilihan) {
            if (pilihan == 1) {
                if (LoginUser()) {
                    running = false; 
                }
            } else if (pilihan == 2) {
                RegisterUser();
            } else if (pilihan == 3) {
                exit(0);
            } else {
                cout << "[ERROR] Pilihan tidak valid." << endl;
                cin.ignore(); cin.get();
            }
        } else {
            cout << "[ERROR] Input harus angka." << endl;
            cin.clear(); cin.ignore(10000, '\n');
            cin.get();
        }
    }
}

// --- LOGIKA BISNIS ---

void TampilkanMenuUtama() {
    system("cls");
    cout << "=======================================================================" << endl;
    cout << "Halo! Selamat Datang " << dataUser[currentUserIndex].NamaLengkap << endl; 
    cout << "Jabatan: " << dataUser[currentUserIndex].Jabatan << " | " << dataUser[currentUserIndex].Email << endl;
    cout << "=======================================================================" << endl;
    
    cout << "\n--- DASHBOARD KESENJANGAN KESEHATAN DIY ---" << endl;
    cout << "1. Proses Analisis Kesenjangan & Peringkat" << endl;
    cout << "2. Cari Detail Wilayah" << endl;
    cout << "3. Tambah Data Wilayah Baru" << endl;
    cout << "4. Tambah Data Faskes Baru" << endl;
    cout << "5. Keluar (Logout)" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Total Data: " << n_wilayah << " Wilayah, " << n_faskes << " Faskes." << endl;
    cout << "Masukkan pilihan Anda (1-5): ";
}

void TambahDataWilayahBaru() {
    if (n_wilayah >= MAX_WILAYAH) {
        cout << "[ERROR] Kapasitas data wilayah penuh." << endl;
        cout << "Tekan Enter..."; cin.ignore(); cin.get();
        return;
    }
    cout << "\n--- Input Data Wilayah Baru ---" << endl;
    if (n_wilayah > 0) {
        cout << "Daftar Wilayah Existing:" << endl;
        for (int i = 0; i < n_wilayah; i++) cout << i + 1 << ". " << dataWilayah[i].Nama << " (" << dataWilayah[i].ID << ")" << endl;
        cout << "---------------------------------------------" << endl;
    }

    string tempID;
    bool idExists;
    do {
        idExists = false;
        cout << "Masukkan ID Wilayah (cth: W001): "; cin >> tempID; tempID = toUpperCase(tempID); 
        for (int i = 0; i < n_wilayah; i++) {
            if (dataWilayah[i].ID == tempID) { cout << "[ERROR] ID Terpakai.\n"; idExists = true; break; }
        }
    } while (idExists);
    dataWilayah[n_wilayah].ID = tempID;
    
    cout << "Masukkan Nama Wilayah (cth: Kabupaten Sleman): "; cin.ignore(); getline(cin, dataWilayah[n_wilayah].Nama);
    dataWilayah[n_wilayah].JumlahPenduduk = InputAngkaFormatTitik("Masukkan Jumlah Penduduk: ");
    
    dataWilayah[n_wilayah].TotalFaskes = 0;
    dataWilayah[n_wilayah].RasioKesenjangan = 0;
    dataWilayah[n_wilayah].Peringkat = 0;
    n_wilayah++;
    cout << "[INFO] Data wilayah baru berhasil ditambahkan." << endl;
    cout << "Tekan Enter untuk lanjut..."; cin.ignore(); cin.get();
}

void TambahDataFaskesBaru() {
    if (n_faskes >= MAX_FASKES) { cout << "[ERROR] Kapasitas Faskes penuh." << endl; cin.ignore(); cin.get(); return; }
    if (n_wilayah == 0) { cout << "[ERROR] Data wilayah kosong." << endl; cin.ignore(); cin.get(); return; }

    cout << "\n--- Input Data Faskes Baru ---" << endl;
    cout << "Daftar Wilayah Tersedia:" << endl;
    for(int i=0; i<n_wilayah; i++) cout << i + 1 << ". " << dataWilayah[i].Nama << " (" << dataWilayah[i].ID << ")" << endl;
    cout << "------------------------" << endl;

    string inputIDWilayah;
    bool validWilayah = false;
    while(!validWilayah) {
        cout << "Masukkan ID Wilayah (cth: W001): "; cin >> inputIDWilayah; inputIDWilayah = toUpperCase(inputIDWilayah); 
        for(int i=0; i<n_wilayah; i++){ if(dataWilayah[i].ID == inputIDWilayah){ validWilayah = true; break; } }
        if(!validWilayah) cout << "[ERROR] ID Wilayah tidak ditemukan." << endl;
    }
    dataFaskes[n_faskes].IDWilayah = inputIDWilayah;

    if (n_faskes > 0) {
        cout << "\n[INFO] ID Faskes Existing:" << endl;
        for (int i = 0; i < n_faskes; i++) cout << i + 1 << ". " << dataFaskes[i].Nama << " (" << dataFaskes[i].ID << ")" << endl;
        cout << "---------------------------------------------" << endl;
    }

    string tempIDF;
    bool idFaskesExists;
    do {
        idFaskesExists = false;
        cout << "Masukkan ID Faskes Baru (cth: F005): "; cin >> tempIDF; tempIDF = toUpperCase(tempIDF);
        for(int i=0; i<n_faskes; i++) { if(dataFaskes[i].ID == tempIDF) { cout << "[ERROR] ID Terpakai.\n"; idFaskesExists = true; break; } }
    } while(idFaskesExists);
    dataFaskes[n_faskes].ID = tempIDF;
    
    cout << "Masukkan Nama Faskes (cth: RS JIH): "; cin.ignore(); getline(cin, dataFaskes[n_faskes].Nama);
    
    int pilihanTipe;
    bool validTipe = false;
    cout << "Pilih Tipe Faskes:\n1. Rumah Sakit\n2. Puskesmas\n3. Klinik" << endl;
    while(!validTipe) {
        cout << "Masukkan pilihan (1-3): ";
        if(cin >> pilihanTipe) {
            if(pilihanTipe==1) { dataFaskes[n_faskes].Tipe="Rumah Sakit"; validTipe=true; }
            else if(pilihanTipe==2) { dataFaskes[n_faskes].Tipe="Puskesmas"; validTipe=true; }
            else if(pilihanTipe==3) { dataFaskes[n_faskes].Tipe="Klinik"; validTipe=true; }
            else cout << "[ERROR] Pilihan salah." << endl;
        } else { cout << "[ERROR] Input harus angka." << endl; cin.clear(); cin.ignore(10000, '\n'); }
    }
    n_faskes++;
    cout << "[INFO] Data Faskes berhasil ditambahkan." << endl;
    cout << "Tekan Enter untuk lanjut..."; cin.ignore(); cin.get();
}

void HitungTotalFaskes() {
    for (int i = 0; i < n_wilayah; i++) dataWilayah[i].TotalFaskes = 0;
    for (int i = 0; i < n_wilayah; i++) {
        for (int j = 0; j < n_faskes; j++) {
            if (dataWilayah[i].ID == dataFaskes[j].IDWilayah) dataWilayah[i].TotalFaskes++;
        }
    }
}

void HitungRasioKesenjangan() {
    for (int i = 0; i < n_wilayah; i++) {
        if (dataWilayah[i].TotalFaskes > 0) dataWilayah[i].RasioKesenjangan = (double)dataWilayah[i].JumlahPenduduk / dataWilayah[i].TotalFaskes;
        else dataWilayah[i].RasioKesenjangan = 9999999.0; 
    }
}

void UrutkanKesenjangan() {
    int i, j, MaxIndex;
    Wilayah temp;
    for (i = 0; i < n_wilayah - 1; i++) {
        MaxIndex = i;
        for (j = i + 1; j < n_wilayah; j++) {
            if (dataWilayah[j].RasioKesenjangan > dataWilayah[MaxIndex].RasioKesenjangan) MaxIndex = j;
        }
        if (MaxIndex != i) { temp = dataWilayah[i]; dataWilayah[i] = dataWilayah[MaxIndex]; dataWilayah[MaxIndex] = temp; }
    }
    for (int k = 0; k < n_wilayah; k++) dataWilayah[k].Peringkat = k + 1;
}

string TentukanKategoriKesenjangan(int peringkat) {
    if (n_wilayah == 0) return "N/A";
    double kuartil = (double)n_wilayah / 4.0;
    if (peringkat <= ceil(kuartil)) return "Tertinggi";
    else if (peringkat <= ceil(kuartil * 2)) return "Sedang Tinggi";
    else if (peringkat <= ceil(kuartil * 3)) return "Normal";
    else return "Rendah";
}

void TampilkanFaskesPerWilayah(string IDWilayah, bool withID) {
    bool first = true;
    for (int j = 0; j < n_faskes; j++) {
        if (dataFaskes[j].IDWilayah == IDWilayah) {
            if (!first) cout << ", ";
            cout << dataFaskes[j].Nama;
            if (withID) cout << " (" << dataFaskes[j].ID << ", " << dataFaskes[j].Tipe << ")";
            else cout << " (" << dataFaskes[j].Tipe << ")";
            first = false;
        }
    }
    if (first) cout << "Tidak ada Faskes yang dihitung.";
}

void TampilkanSemuaDetailFaskes() {
    cout << "\n=========================================================================" << endl;
    cout << "           DATA RINCI FASILITAS KESEHATAN (ALL TYPES) DIY" << endl;
    cout << "=========================================================================" << endl;
    for(int i = 0; i < n_wilayah; i++) {
        cout << "| " << setw(28) << left << dataWilayah[i].Nama << " | Total Faskes: " << dataWilayah[i].TotalFaskes << endl;
        cout << "  - Faskes: "; TampilkanFaskesPerWilayah(dataWilayah[i].ID, true); cout << endl;
        cout << "-------------------------------------------------------------------------" << endl;
    }
}

void TampilkanPeringkat() {
    cout << "\n=======================================================================================================" << endl;
    cout << "                                  TABEL PERINGKAT KESENJANGAN FASKES DIY" << endl;
    cout << "=======================================================================================================" << endl;
    cout << "Rank | Wilayah (ID) \t\t\t| Populasi \t| Total Faskes | Rasio P/F \t\t| Kategori Kesenjangan" << endl;
    cout << "-------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < n_wilayah; i++) {
        cout << left << setw(4) << (i + 1) << "| ";
        cout << setw(28) << (dataWilayah[i].Nama + " (" + dataWilayah[i].ID + ")") << "| ";
        cout << setw(12) << FormatAngkaRibuan(dataWilayah[i].JumlahPenduduk) << "| ";
        cout << setw(12) << dataWilayah[i].TotalFaskes << "| ";
        if (dataWilayah[i].RasioKesenjangan > 999999.0) cout << left << setw(16) << "TAK TERHINGGA" << "| ";
        else cout << fixed << setprecision(2) << setw(16) << dataWilayah[i].RasioKesenjangan << "| ";
        cout << TentukanKategoriKesenjangan(dataWilayah[i].Peringkat) << endl;
    }
    cout << "=======================================================================================================" << endl;
}

void CariDetailWilayahSekuensial(string NamaTarget) {
    bool found = false; int index = -1;
    for (int i = 0; i < n_wilayah; i++) { if (dataWilayah[i].Nama == NamaTarget) { found = true; index = i; break; } }

    if (found) {
        cout << "\n--- HASIL PENCARIAN SEKUENSIAL ---" << endl;
        cout << "Nama Wilayah\t\t: " << dataWilayah[index].Nama << endl;
        cout << "Populasi\t\t: " << FormatAngkaRibuan(dataWilayah[index].JumlahPenduduk) << endl;
        cout << "Total Faskes\t\t: " << dataWilayah[index].TotalFaskes << endl;
        cout << "Rasio Kesenjangan\t: " << fixed << setprecision(2) << dataWilayah[index].RasioKesenjangan << endl;
        cout << "Peringkat Kesenjangan\t: " << dataWilayah[index].Peringkat << endl;
        cout << "Kategori Kesenjangan\t: " << TentukanKategoriKesenjangan(dataWilayah[index].Peringkat) << endl;
        cout << "[Faskes yang Dihitung]: "; TampilkanFaskesPerWilayah(dataWilayah[index].ID, true); cout << endl;
        cout << "--------------------------------------" << endl;
    } else { cout << "\n[INFO] Wilayah tidak ditemukan." << endl; }
}

int main() {
    HalamanAwal(); // MASUK KE LOGIN/REGISTER

    int pilihan;
    string cariNama;
    char pilihanDetail;
    
    do {
        TampilkanMenuUtama();
        if (!(cin >> pilihan)) {
            cout << "\n[ERROR] Input harus angka!" << endl;
            cin.clear(); cin.ignore(10000, '\n'); pilihan = 0; continue;
        }
        
        if (pilihan == 1) {
            if (n_wilayah == 0) { 
                cout << "\n[ERROR] Data wilayah kosong. Input data dulu di menu 3." << endl; 
                cout << "Tekan Enter..."; cin.ignore(); cin.get(); continue; 
            }
            cout << "\n[PROSES] Menghitung..." << endl;
            HitungTotalFaskes(); HitungRasioKesenjangan(); UrutkanKesenjangan(); TampilkanPeringkat();
            
            cout << "\nLihat detail faskes? (Y/T): "; cin >> pilihanDetail;
            if (toupper(pilihanDetail) == 'Y') {
                TampilkanSemuaDetailFaskes();
                // FIX: Pause agar tidak langsung clear screen
                cout << "\nTekan Enter untuk kembali ke dashboard...";
                cin.ignore(); cin.get();
            }
        } else if (pilihan == 2) {
            if (n_wilayah == 0) { 
                cout << "\n[ERROR] Data wilayah kosong." << endl; 
                cout << "Tekan Enter..."; cin.ignore(); cin.get(); continue; 
            }
            HitungTotalFaskes(); HitungRasioKesenjangan(); UrutkanKesenjangan();
            cout << "\n--- Daftar Wilayah ---" << endl;
            for (int i = 0; i < n_wilayah; i++) cout << i + 1 << ". " << dataWilayah[i].Nama << endl;
            cout << "Masukkan NAMA Wilayah dicari: "; cin.ignore(); getline(cin, cariNama);
            CariDetailWilayahSekuensial(cariNama);
            
            // FIX: Pause agar hasil pencarian terbaca
            cout << "\nTekan Enter untuk kembali..."; cin.get();
        } else if (pilihan == 3) { 
            TambahDataWilayahBaru(); 
        } else if (pilihan == 4) { 
            TambahDataFaskesBaru(); 
        } else if (pilihan == 5) { 
            cout << "[LOGOUT] Anda berhasil keluar." << endl;
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
            currentUserIndex = -1;
            main(); // Restart ke login
            return 0;
        } else { 
            cout << "\n[ERROR] Pilihan tidak valid." << endl; 
            cout << "Tekan Enter..."; cin.ignore(); cin.get();
        }

    } while (pilihan != 5); 

    return 0;
}