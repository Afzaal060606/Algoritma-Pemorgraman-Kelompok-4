#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath> 

using namespace std;

const int MAX_WILAYAH = 10;
const int MAX_FASKES = 40; 

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

Wilayah dataWilayah[MAX_WILAYAH];
Faskes dataFaskes[MAX_FASKES];

int n_wilayah = 0; 
int n_faskes = 0; 


void TampilkanMenuUtama();
void HitungTotalFaskes();
void HitungRasioKesenjangan();
void UrutkanKesenjangan(); 
string TentukanKategoriKesenjangan(int peringkat); 
void CariDetailWilayahSekuensial(string NamaTarget); 
void TampilkanPeringkat();
void TampilkanSemuaDetailFaskes();
void TampilkanFaskesPerWilayah(string IDWilayah, bool withID); 


void TambahDataWilayahBaru();
void TambahDataFaskesBaru();


void TampilkanMenuUtama() {
    cout << "\n--- SISTEM ANALISIS KESENJANGAN FASKES DIY ---" << endl;
    cout << "1. Proses Analisis Kesenjangan & Peringkat" << endl;
    cout << "2. Cari Detail Wilayah" << endl;
    cout << "3. Tambah Data Wilayah Baru" << endl;
    cout << "4. Tambah Data Faskes Baru" << endl;
    cout << "5. Keluar Program" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Total Data: " << n_wilayah << " Wilayah, " << n_faskes << " Faskes." << endl;
    cout << "Masukkan pilihan Anda (1-5): ";
}

void TambahDataWilayahBaru() {
    if (n_wilayah >= MAX_WILAYAH) {
        cout << "[ERROR] Kapasitas data wilayah penuh (Maks: " << MAX_WILAYAH << ")." << endl;
        return;
    }
    
    cout << "\n--- Input Data Wilayah Baru ---" << endl;
    cout << "Masukkan ID Wilayah (cth: W001): ";
    cin >> dataWilayah[n_wilayah].ID;
    
    cout << "Masukkan Nama Wilayah (cth: Kabupaten Sleman): ";
    cin.ignore(); 
    getline(cin, dataWilayah[n_wilayah].Nama);
    
    cout << "Masukkan Jumlah Penduduk: ";
    cin >> dataWilayah[n_wilayah].JumlahPenduduk;
    
    dataWilayah[n_wilayah].TotalFaskes = 0;
    dataWilayah[n_wilayah].RasioKesenjangan = 0;
    dataWilayah[n_wilayah].Peringkat = 0;
    
    n_wilayah++; 
    cout << "[INFO] Data wilayah baru berhasil ditambahkan. Total: " << n_wilayah << " wilayah." << endl;
}

void TambahDataFaskesBaru() {
    if (n_faskes >= MAX_FASKES) {
        cout << "[ERROR] Kapasitas data Faskes penuh (Maks: " << MAX_FASKES << ")." << endl;
        return;
    }
    
    if (n_wilayah == 0) {
         cout << "[ERROR] Harap isi data wilayah terlebih dahulu (Menu 3)." << endl;
         return;
    }

    cout << "\n--- Input Data Faskes Baru ---" << endl;
    cout << "Masukkan ID Faskes (cth: F001): ";
    cin >> dataFaskes[n_faskes].ID;
    
    cout << "Masukkan Nama Faskes (cth: RS JIH): ";
    cin.ignore();
    getline(cin, dataFaskes[n_faskes].Nama);
    
    cout << "Masukkan ID Wilayah (cth: W001): "; 
    cin >> dataFaskes[n_faskes].IDWilayah;
    
    cout << "Masukkan Tipe Faskes (Puskesmas/Rumah Sakit/Klinik): ";
    cin.ignore();
    getline(cin, dataFaskes[n_faskes].Tipe);
    
    n_faskes++;
    cout << "[INFO] Data Faskes baru berhasil ditambahkan. Total: " << n_faskes << " Faskes." << endl;
}


void HitungTotalFaskes() {
    for (int i = 0; i < n_wilayah; i++) {
        dataWilayah[i].TotalFaskes = 0;
    }
    for (int i = 0; i < n_wilayah; i++) {
        for (int j = 0; j < n_faskes; j++) {
            if (dataFaskes[j].Tipe == "Puskesmas" || dataFaskes[j].Tipe == "Rumah Sakit") {
                if (dataWilayah[i].ID == dataFaskes[j].IDWilayah) {
                    dataWilayah[i].TotalFaskes++;
                }
            }
        }
    }
}

void HitungRasioKesenjangan() {
    for (int i = 0; i < n_wilayah; i++) {
        if (dataWilayah[i].TotalFaskes > 0) {
            dataWilayah[i].RasioKesenjangan = (double)dataWilayah[i].JumlahPenduduk / dataWilayah[i].TotalFaskes;
        } else {
            dataWilayah[i].RasioKesenjangan = 9999999.0; 
        }
    }
}

void UrutkanKesenjangan() {
    int i, j, MaxIndex;
    Wilayah temp;

    for (i = 0; i < n_wilayah - 1; i++) {
        MaxIndex = i;
        for (j = i + 1; j < n_wilayah; j++) {
            if (dataWilayah[j].RasioKesenjangan > dataWilayah[MaxIndex].RasioKesenjangan) {
                MaxIndex = j;
            }
        }
        if (MaxIndex != i) {
            temp = dataWilayah[i];
            dataWilayah[i] = dataWilayah[MaxIndex];
            dataWilayah[MaxIndex] = temp;
        }
    }
    
    for (int k = 0; k < n_wilayah; k++) {
        dataWilayah[k].Peringkat = k + 1;
    }
}

string TentukanKategoriKesenjangan(int peringkat) {
    if (n_wilayah == 0) return "N/A";

    double kuartil = (double)n_wilayah / 4.0;
    
    if (peringkat <= ceil(kuartil)) {
        return "Tertinggi";
    } else if (peringkat <= ceil(kuartil * 2)) {
        return "Sedang Tinggi";
    } else if (peringkat <= ceil(kuartil * 3)) {
        return "Normal";
    } else {
        return "Rendah";
    }
}

void TampilkanFaskesPerWilayah(string IDWilayah, bool withID) {
    bool first = true;
    for (int j = 0; j < n_faskes; j++) {
        if ((dataFaskes[j].Tipe == "Puskesmas" || dataFaskes[j].Tipe == "Rumah Sakit") && dataFaskes[j].IDWilayah == IDWilayah) {
            if (!first) {
                cout << ", ";
            }
            cout << dataFaskes[j].Nama;
            
            if (withID) {
                cout << " (" << dataFaskes[j].ID << ", " << dataFaskes[j].Tipe << ")";
            } else {
                 cout << " (" << dataFaskes[j].Tipe << ")";
            }
            first = false;
        }
    }
    if (first) {
         cout << "Tidak ada Faskes yang dihitung.";
    }
}

void TampilkanSemuaDetailFaskes() {
    cout << "\n=========================================================================" << endl;
    cout << "           DATA RINCI FASILITAS KESEHATAN (RS/PUSKESMAS) DIY" << endl;
    cout << "=========================================================================" << endl;
    
    for(int i = 0; i < n_wilayah; i++) {
        cout << "| " << setw(28) << left << dataWilayah[i].Nama << " | Total Faskes: " << dataWilayah[i].TotalFaskes << endl;
        cout << "  - Faskes: ";
        TampilkanFaskesPerWilayah(dataWilayah[i].ID, true);
        cout << endl;
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
        cout << setw(12) << dataWilayah[i].JumlahPenduduk << "| ";
        cout << setw(12) << dataWilayah[i].TotalFaskes << "| ";
        
        if (dataWilayah[i].RasioKesenjangan > 999999.0) {
             cout << left << setw(16) << "TAK TERHINGGA" << "| ";
        } else {
             cout << fixed << setprecision(2) << setw(16) << dataWilayah[i].RasioKesenjangan << "| ";
        }
        
        cout << TentukanKategoriKesenjangan(dataWilayah[i].Peringkat) << endl;
    }
    cout << "=======================================================================================================" << endl;
}

void CariDetailWilayahSekuensial(string NamaTarget) {
    bool found = false;
    int index = -1;

    for (int i = 0; i < n_wilayah; i++) {
        if (dataWilayah[i].Nama == NamaTarget) { 
            found = true;
            index = i;
            break;
        }
    }

    if (found) {
        cout << "\n--- HASIL PENCARIAN SEKUENSIAL ---" << endl;
        cout << "Nama Wilayah\t\t: " << dataWilayah[index].Nama << endl;
        cout << "Populasi\t\t: " << dataWilayah[index].JumlahPenduduk << endl;
        cout << "Total Faskes\t\t: " << dataWilayah[index].TotalFaskes << endl;
        cout << "Rasio Kesenjangan\t: " << fixed << setprecision(2) << dataWilayah[index].RasioKesenjangan << endl;
        cout << "Peringkat Kesenjangan\t: " << dataWilayah[index].Peringkat << endl;
        cout << "Kategori Kesenjangan\t: " << TentukanKategoriKesenjangan(dataWilayah[index].Peringkat) << endl;
        cout << "[Faskes yang Dihitung]: ";
        TampilkanFaskesPerWilayah(dataWilayah[index].ID, true); 
        cout << endl;
        cout << "--------------------------------------" << endl;
    } else {
        cout << "\n[INFO] Wilayah dengan nama '" << NamaTarget << "' tidak ditemukan. Pastikan penulisan sudah benar." << endl;
    }
}

int main() {
    int pilihan;
    string cariNama;
    char pilihanDetail;
    
    do {
        TampilkanMenuUtama();
        
        if (!(cin >> pilihan)) {
            cout << "\n[ERROR] Input harus angka!" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            pilihan = 0;
            continue;
        }
        
        if (pilihan == 1) {
            if (n_wilayah == 0) {
                cout << "\n[ERROR] Data wilayah masih kosong. Silakan isi data (Menu 3) terlebih dahulu." << endl;
                continue; 
            }
            
            cout << "\n[PROSES] Menghitung Total Faskes dan Rasio..." << endl;
            HitungTotalFaskes();
            HitungRasioKesenjangan();
            
            cout << "[PROSES] Mengurutkan peringkat dengan Selection Sort dan menentukan kategori..." << endl;
            UrutkanKesenjangan();
            
            TampilkanPeringkat();
            
            cout << "\nApakah Anda ingin melihat data fasilitas kesehatan di Yogyakarta? (Y/T): ";
            cin >> pilihanDetail;
            pilihanDetail = toupper(pilihanDetail);
            
            if (pilihanDetail == 'Y') {
                TampilkanSemuaDetailFaskes();
            }
            
        } else if (pilihan == 2) {
            if (n_wilayah == 0) {
                cout << "\n[ERROR] Data wilayah masih kosong. Tidak ada yang bisa dicari." << endl;
                continue;
            }
            
            HitungTotalFaskes();
            HitungRasioKesenjangan();
            UrutkanKesenjangan();
            
            cout << "Masukkan NAMA Wilayah yang dicari: ";
            cin.ignore(); 
            getline(cin, cariNama);
            
            CariDetailWilayahSekuensial(cariNama);
            
        } else if (pilihan == 3) {
            TambahDataWilayahBaru();
            
        } else if (pilihan == 4) {
            TambahDataFaskesBaru();
            
        } else if (pilihan != 5) { 
            cout << "\n[ERROR] Pilihan tidak valid. Silakan masukkan 1-5." << endl;
        }

    } while (pilihan != 5); 

    cout << "\n[INFO] Program selesai. Terima kasih." << endl;
    return 0;
}