#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define NAMA_FILE "HabitTracker.txt"

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BLUE    "\033[1;34m"
#define WHITE   "\033[1;37m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

typedef struct {
    char nama[100];
    char hariTanggal[50];
    char jam[10];
    char durasi[20];
    char deadline[30];
    char keterangan[250];
    int selesai; // 0 = belum selesai, 1 = selesai
} TabelKegiatan;

TabelKegiatan list[MAX];
int jumlah = 0;

// ---------------------- FILE HANDLING ----------------------
void save() {
    FILE *fp = fopen("HabitTracker.txt", "w");
    if (!fp) {
        printf(RED "Gagal menyimpan ke File Habit Tracker!\n" RESET);
        return;
    }

    for (int i = 0; i < jumlah; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%d\n",
                list[i].nama,
                list[i].hariTanggal,
                list[i].jam,
                list[i].durasi,
                list[i].deadline,
                list[i].keterangan,
                list[i].selesai);
    }
    fclose(fp);
}

void read() {
    FILE *fp = fopen("HabitTracker.txt", "r");
    if (!fp) return;

    jumlah = 0;
    while (fscanf(fp, " %99[^|]|%49[^|]|%9[^|]|%19[^|]|%29[^|]|%249[^|]|%d\n",
                  list[jumlah].nama,
                  list[jumlah].hariTanggal,
                  list[jumlah].jam,
                  list[jumlah].durasi,
                  list[jumlah].deadline,
                  list[jumlah].keterangan,
                  &list[jumlah].selesai) == 7) {
        jumlah++;
        if (jumlah >= MAX) break;
    }
    fclose(fp);
}

// ---------------------- TAMBAH KEGIATAN ----------------------
void inputKegiatan() {
    if (jumlah >= MAX) {
        printf(RED "Kegiatan penuh!\n" RESET);
        return;
    }

    getchar();
    printf(MAGENTA "Masukkan Nama Kegiatan      : " RESET);
    fgets(list[jumlah].nama, sizeof(list[jumlah].nama), stdin);
    list[jumlah].nama[strcspn(list[jumlah].nama, "\n")] = 0;

    printf(CYAN "Masukkan Hari, Tanggal      : " RESET);
    fgets(list[jumlah].hariTanggal, sizeof(list[jumlah].hariTanggal), stdin);
    list[jumlah].hariTanggal[strcspn(list[jumlah].hariTanggal, "\n")] = 0;

    printf(MAGENTA "Masukkan Jam (HH:MM)        : " RESET);
    fgets(list[jumlah].jam, sizeof(list[jumlah].jam), stdin);
    list[jumlah].jam[strcspn(list[jumlah].jam, "\n")] = 0;

    printf(CYAN "Masukkan Durasi Kegiatan (cth : 5 menit)   : " RESET);
    fgets(list[jumlah].durasi, sizeof(list[jumlah].durasi), stdin);
    list[jumlah].durasi[strcspn(list[jumlah].durasi, "\n")] = 0;

    printf(MAGENTA "Masukkan Deadline           : " RESET);
    fgets(list[jumlah].deadline, sizeof(list[jumlah].deadline), stdin);
    list[jumlah].deadline[strcspn(list[jumlah].deadline, "\n")] = 0;

    printf(CYAN "Masukkan Keterangan (opsional)        : " RESET);
    fgets(list[jumlah].keterangan, sizeof(list[jumlah].keterangan), stdin);
    list[jumlah].keterangan[strcspn(list[jumlah].keterangan, "\n")] = 0;

    list[jumlah].selesai = 0;
    jumlah++;

    save();
    printf(GREEN "Kegiatan berhasil ditambahkan!\n" RESET);
}

// ---------------------- LIST KEGIATAN ----------------------
void listKegiatan() {
    printf(BLUE "================================================================================"RESET);
    printf(BLUE "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DAILY HABIT TRACKER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" RESET);
    printf(BLUE "================================================================================\n\n"RESET);

    // Setting lebar kolom
    printf(CYAN "%-4s %-20s %-25s %-10s %-12s %-20s %-4s\n" RESET,
           "No", "Nama Kegiatan", "Hari & Tanggal", "Jam", "Durasi", "Deadline", "Keterangan");

    for (int i = 0; i < jumlah; i++) {
        printf("%-4d %-20s %-25s %-10s %-12s %-20s %-10s\n",
               i + 1,
               list[i].nama,
               list[i].hariTanggal,
               list[i].jam,
               list[i].durasi,
               list[i].deadline,
               list[i].keterangan);
    }
}

// ---------------------- CHECKLIST ----------------------
void checklist() {
    listKegiatan();
    int done;

    printf(YELLOW"\nPilih nomor kegiatan yang ingin ditandai selesai: "RESET);
    scanf("%d", &done);
    done--;

    if (done < 0 || done >= jumlah) {
        printf(RED "Nomor tidak valid!\n" RESET);
        return;
    }

    list[done].selesai = 1;
    save();
    printf(GREEN "Kegiatan berhasil ditandai selesai!\n" RESET);
}

// ---------------------- EDIT KEGIATAN ----------------------
void editKegiatan() {
    if (jumlah == 0) {
        printf(RED "Kegiatan kosong, silahkan input kegiatan!\n" RESET);
        return;
    }

    listKegiatan();
    int edit;

    printf(YELLOW"\nPilih nomor kegiatan yang ingin diedit: "RESET);
    scanf("%d", &edit);
    getchar();
    edit--;

    if (edit < 0 || edit >= jumlah) {
        printf(RED "Nomor tidak valid!\n" RESET);
        return;
    }
    printf(MAGENTA "Nama Kegiatan baru  : " RESET);

    char input[100];
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(list[edit].nama, input);
    }

    printf(MAGENTA "Hari & Tanggal baru    : " RESET);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(list[edit].hariTanggal, input);
    }

    printf(MAGENTA "Jam baru (HH:MM)    : " RESET);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(list[edit].jam, input);
    }

    printf(MAGENTA "Durasi baru    : " RESET);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(list[edit].durasi, input);
    }

    printf(MAGENTA "Deadline baru   : " RESET);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(list[edit].deadline, input);
    }

    printf(MAGENTA "Keterangan baru    : " RESET);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        input[strcspn(input, "\n")] = 0;
        strcpy(list[edit].keterangan, input);
    }

    save();
    printf(GREEN "Kegiatan berhasil diedit!\n" RESET);
}

// ---------------------- HAPUS KEGIATAN ----------------------
void hapusKegiatan() {
    if (jumlah == 0) {
        printf(RED "\nTidak ada kegiatan untuk dihapus!\n" RESET);
        return;
    }

    listKegiatan();
    int delete;
    printf(YELLOW"\nMasukkan nomor kegiatan yang ingin dihapus: "RESET);
    scanf("%d", &delete);
    delete--;

    if (delete < 0 || delete >= jumlah) {
        printf(RED "Nomor tidak valid!\n" RESET);
        return;
    }

    for (int i = delete; i < jumlah - 1; i++) {
        list[i] = list[i + 1];
    }
    jumlah--;

    save();
    printf(GREEN "Kegiatan berhasil dihapus!\n" RESET);
}

// ---------------------- MENU ----------------------
void menu() {
    read();
    int pilihan;
    do {
        printf(BLUE"\n===== HABIT TRACKER =====\n"RESET);
        printf(WHITE"1. Tambah Kegiatan\n"RESET);
        printf(WHITE"2. Lihat List Kegiatan\n"RESET);
        printf(WHITE"3. Tandai Selesai\n"RESET);
        printf(WHITE"4. Edit Kegiatan\n"RESET);
        printf(WHITE"5. Hapus Kegiatan\n"RESET);
        printf(WHITE"0. Keluar\n"RESET);
        printf(GREEN"\nSilahkan pilih : "RESET);
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: inputKegiatan(); break;
            case 2: listKegiatan(); break;
            case 3: checklist(); break;
            case 4: editKegiatan(); break;
            case 5: hapusKegiatan(); break;
            case 0: printf(RED"Keluar...\n"RESET); break;
            default: printf(RED "Pilihan tidak valid!\n" RESET);
        }
    } while (pilihan != 0);
}

int main() {
    menu();
    return 0;
}