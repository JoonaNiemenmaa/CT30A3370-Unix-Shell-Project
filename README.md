# Projekti III: Unix Shell

Toiseksi projektiksi tein Unix Shell:in eli komentorivipäätteen. Lähdekoodi löytyy täältä. Projekti koostuu kolmesta moduulista, path, vector ja wish. Wish sisältää ohjelman pääohjelman ja sisältää shell:in päätoiminnallisuuden. Vector implementoi vektori-tietorakenteen merkkijonoille ja path sisältää ohjelman path-muuttujan hallitsemiseen liittyvät funktiot.

Tässä pieni kaavio ohjelman rakenteesta:

<img width="750" height="403" alt="image" src="https://github.com/user-attachments/assets/8bbdba54-6f7c-4637-9c0c-33d400d921f6" />

Seuraavaksi kuvankaappauksia ohjelman käytöstä.

Tässä on ohjelman perustoiminnallisuus eli kyky ajaa ohjelmia komentoriviltä:

<img width="1296" height="350" alt="image" src="https://github.com/user-attachments/assets/a31d0907-a06f-4955-b363-50bfd5b6ec5b" />

Seuraavaksi kuvankaappaukset sisäänrakennetuista komennoista cd, path, ja exit:

<img width="871" height="174" alt="image" src="https://github.com/user-attachments/assets/adf38e5a-34b1-48d0-8db6-650159280cdc" />

<img width="1281" height="234" alt="image" src="https://github.com/user-attachments/assets/78339120-ca54-4469-9b7c-c0db473fb057" />

<img width="823" height="81" alt="image" src="https://github.com/user-attachments/assets/fd64252f-623b-41b5-a673-1107b91d8573" />


Ohjelma pystyy myös suorittamaan komentoja sille syötetystä tiedostosta. Kuvankaappauksesta myös näkee kuinka stdout:in uudelleensuuntaus toimii:

<img width="1285" height="447" alt="image" src="https://github.com/user-attachments/assets/daad3466-d90e-41f4-8cf8-e5a3c90ec881" />

Ohjelmia voi ajaa rinnakkain erottamalla niiden komennot &-merkillä. Kuvan oikealla puolella näkyy btop-ohjelma joka listaa koneella ajossa olevat prosessit.
 
<img width="1310" height="354" alt="image" src="https://github.com/user-attachments/assets/6d79b02f-2f6c-4c60-8305-99abdd99720a" />

## Käyttö

Ohjelma käännetään make:lla. Make-skripti käyttää gcc:tä kääntäjänä.

```console
foo@bar:~$ make
```
Make-skripti luo ajettavan ohjelman wish, joka ajetaan seuraavasti:

```console
foo@bar:~$ ./wish
```
Ohjelma ajetaan batch-tilassa seuraavasti:

```console
foo@bar:~$ ./wish <tiedostonimi>
```
