### Ustawienia Ogólne

- **WYSWIETLAJ_PASEK_POSTEPU**: 1/0, pokazuje pasek postępu.
- **PLIK_WYNIKOWY**: plik z raportem z testu pojedynczego pliku.
- **PLIK_RAPORTU_GENEROWANEGO**: plik z raportem z testu folderu.
- **PLIK_KONWERGENCJI**: plik z przebiegiem konwergencji.
- **POWTORZENIA**: ile razy liczyć tę samą instancję.

### Algorytm Genetyczny

- **GA_ROZMIAR_POPULACJI**: wielkość populacji.
- **GA_MAX_POKOLEN**: maksymalna liczba pokoleń.
- **GA_WSP_KRZYZOWANIA**: współczynnik krzyżowania 
- **GA_METODA_KRZYZOWANIA**: OX lub PMX.
- **GA_WSP_MUTACJI**: współczynnik mutacji 
- **GA_METODA_MUTACJI**: INW  lub SCRAMBLE.
- **GA_ELITARYZM**: liczba osobników elity.
- **GA_ROZMIAR_TURNIEJU**: rozmiar turnieju w selekcji.

### Tryb Testów

- **TRYB_TESTOW**: PLIK lub FOLDER.
- **SCIEZKA_DO_FOLDERU**: gdzie są macierze (dla FOLDER).
- **SCIEZKA_DO_PLIKU**: plik z macierzą (dla PLIK).

## Przykładowa Konfiguracja

WYSWIETLAJ_PASEK_POSTEPU = 1
PLIK_WYNIKOWY = wyniki/raport_wyniki.csv
PLIK_KONWERGENCJI = wyniki/raport_konwergencja.csv
POWTORZENIA = 3

GA_ROZMIAR_POPULACJI = 100
GA_MAX_POKOLEN = 10000
GA_WSP_KRZYZOWANIA = 0.80
GA_METODA_KRZYZOWANIA = OX
GA_WSP_MUTACJI = 0.05
GA_METODA_MUTACJI = INW
GA_ELITARYZM = 1
GA_ROZMIAR_TURNIEJU = 3

SCIEZKA_DO_PLIKU = dane/asymetryczne/rbg403.atsp.txt
