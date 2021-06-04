# Smart sprinkler

### Automatyczny system podlewający. 
 <br>

## Cel

Celem zadania było utworzenie systemu umożliwiającego automatyczne podlewanie ogrodu. Podlewanie powinno móc odbywać się na żądanie użytkownika (włączenie/wyłączenie pompy zdalnie przy pomocy interfejsu) oraz automatycznie, na podstawie zdefiniowanej strategii.  

Strategia może zostać utworzona w oparciu o dane z sensorów oraz progonzę pogody dla danego miejsca (pobieraną z zewnętrznego serwisu). W strategii można określić jakie warunki mają zostać spełnione, aby podlewanie rozpoczeło się oraz jak długo powinno trwać. 

System powinien udostępniać interfejs, dostępny z każdego urządzenia w sieci lokalnej.
Przydatnym elementem systemu byłby również zdalny czujnik wilgotności gleby, oddzielony od głównego układu.

## Platforma sprzętowa 
Urządzenie składa się z: 
* mikrokontrolera ESP8266
* modułu czytnika kart SD
* czujnika temperatury oraz wilgotności powietrza DHT11
* rezystancyjnego czujnika wilgotności gleby
* diód informujących o stanie urządzenia
* modułu wykonawczego (przekaźnika/diody LED)
 
![platforma-sprzetowa](/img/uklad.png)

## Architektura systemu  
Sercem systemu jest mikrokontroler z modułem WiFi, który pobiera  dane z czujników oraz prognozę pogody z otwartego serwisu pogodowego OpenWeather, a następnie przetwarza te dane na podstawie zdefiniowanych przez użytkownika reguł. Użytkownik poprzez interfejs webowy może sterować całym urządzeniem.  
  
Z serwisu OpenWeather pobierane są dane pogodowe na najbliższe 24 godziny dla określonej lokalizacji. Następnie dane są selekcjonowane i tylko wybrane informacje zapisujemy do pamięci urządzenia.   
  
Definiowanie nowej strategii podlewania odbywa się poprzez panel webowy. Utworzona strategia jest wysyłana do serwera, który zapisuje ją na karcie SD w postaci pliku JSON.   
Cała komunikacja między backendem i frontendem opiera się na zapytaniach REST API.  
  
Strona internetowa hostowana jest przez mikrokontroler, ponieważ wymaganiem projektu była niezależność od zewnętrznych usług serwerowych oraz w miarę proste uruchomienie systemu przez użytkownika końcowego. Z racji, iż chcieliśmy zaimplementować nowoczesny interfejs webowy, którego rozmiar mógłby przekroczyć możliwości pamięci wewnętrznej mikrokontrolera, do przetrzymywania plików statycznych strony została wykorzystana karta SD. Zbudowany panel webowy z wykorzystaniem biblioteki React.js zajmował ~300kB pamięci, co biorąc pod uwagę ograniczone zasoby sprzętowe mikrokontrolera dawało czas ładowania interfejsu na poziomie 10s. Po kilku próbach udało nam się zredukować rozmiar pliku do ~20kB przerzucając obowiązek ładowania bibliotek na przeglądarkę użytkownika, korzystając z CDNów udostępniających biblioteki. Pozwoliło to zmniejszyć czas ładowania do ~2s (z wykorzystaniem ładowania bibliotek z pamięci podręcznej przeglądarki). Następnie dzięki temu, iż pliki html oraz js są statyczne, istnieje możliwość zapamiętywania ich przez przeglądarkę użytkownika (po ustawieniu odpowiednich nagłówków w odpowiedzi HTTP), w wyniku czego czas ładowania stał się prawie natychmiastowy (~1s), co jest wartością w pełni akceptowalną.  

## Implementacja programu mikrokontrolera 
Kod mikrokontrolera składa się z kilku głównych klas odpowiedzialnych za poszczególne funkcje: 

* ClockProvider - synchronizuje czas poprzez protokół NTP oraz dostarcza go go do innych klas. 

* CommandParser - parsuje komendy przychodzące na port szeregowy oraz wykonuje odpowiednie akcje.  

* GeneralLogger - pozwala innym klasom na logowanie komunikatów o różnym poziomie priorytetu oraz w przypadku komunikatów ERROR zapala czerwoną diodę informując o wystąpieniu błędu. 

* LogicExecutor - zarządza strategiami użytkownika: wczytuje z plików, zapisuje, interpretuje oraz wykonuje. 

* OutputModule - udostępnia abstrakcję wejścia oraz wyjścia  dla innych klas np. włącza/wyłącza pompę, pobiera dane z czujników. 

* SDCardManager - udostępnia abstrakcję do zarządzania plikami na karcie SD. 

* SettingsManager - zarządza ustawieniami urządzenia (np. dane do połączenia z WiFi), udostępnia je innym klasom oraz zapisuje i wczytuje do pamięci wewnętrznej FLASH. 

* WeatherAPI - odpowiada za komunikacje z API pogodowym, parsowanie oraz interpretację tych danych. 

* WebServer - odpowiada za komunikację HTTP. 

* WiFiConnector - utrzymuje połączenia z WiFi. 

Zadania które powinny wykonywać się z określoną częstotliwością zorganizowane są w programowe wątki.  W funkcji setup() tworzone są instancje wszystkich klas oraz konfigurowane są poszczególne wątki programu. W pętli głównej, co określony czas, wykonywane są funkcje różnych klas, które mają sprawdzić czy trzeba obsłużyć jakieś zdarzenie (zapytanie HTTP, komendę w porcie szeregowym, uruchomienie zadania po upływie określonego czasu). 


## Strategie użytkownika 
Strategie zorganizowane są w bloki wykonujące przetwarzanie potokowe. Każdy element takiego bloku może być typu: 

* condition_check - określa jeden warunek strategii (np. soil_humidity<=50) 

* connector - określa sposób połączenia między warunkami (condition_check) strategii, a jego wartość to and lub or 

* result - określa ile czasu powinno trwać podlewanie 

Strategie są przekazywane (z frontendu do backendu) oraz są przechowywane w postaci plików JSON. Dla zmniejszenia rozmiaru typy oraz parametry poszczególnych elementów zamieniane są na liczby. 
Aby strategia została odpowiednio przetworzona pomiędzy każdym z warunków (condition_check) powinien znaleźć się element łączący (connector), a jako ostatni element strategii powinna znaleźć się informacja o czasie podlewania (result).  
Podczas wykonywania strategii odczytywany jest odpowiedni plik, następnie jest on parsowany i  interpretowany element po elemencie.

## Interfejs webowy 
Zarządzanie urządzeniem odbywa się poprzez panel webowy udostępniany przez moduł ESP8266, który pełni funkcję serwera HTTP. Interfejs został napisany z wykorzystaniem bibliotek REACT.js oraz Material-UI. 

Strona internetowa umożliwia monitorowanie stanu podlewania, obserwację danych zbieranych przez sensory (temperatura powietrza, wilgotność powietrza, wilgotność gleby) oraz zinterpretowanych danych z serwisu pogodowego (m.in. maksymalna temperatura powietrza oraz maksymalne prawdopodobieństwo opadów w ciągu dwudziestu czterech godzin). Użytkownik może wyłączyć podlewanie “na żądanie”, jak i włączyć, ustalając czas nawadniania. 
Przy pomocy interfejsu możemy zobaczyć zdefiniowane strategie podlewania,  zarządzać ich stanem (włączenie/wyłączenie), usunąć wybraną strategię oraz  zdefiniować nową. (Być może będziemy mogli również edytować istniejącą strategię). 

Stan podlewania jest odświeżany co pięć sekund, a wartość sensorów co minutę.  
 
 ![dashboard](/img/dashboard.png) 
 
