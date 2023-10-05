# OpenGL-Pirate-Game
**Descriere generala**

Scena 2D este reprezentată de o corabie de pirați, marea, insule cu palmieri, cerul, soarele și scorul afișat. Corabia de pirați se poate mișca pe axa OX cu ajutorul mouse-ului. De la limita superioară a axei OY și de pe poziții random în raport cu axa OX vor apărea obiecte de diferite tipuri în mod random care se vor mișca în jos pe axa OY. Obiectele care pot apărea în mod random sunt reprezentate de o sticlă de rom, o monedă, un cufăr și o ghiulea. Obiectivul jucătorului este să culeagă cât mai multe cufere, sticle și monede, dar în același timp să se ferească de ghiulea. Atingerea unei ghiulele presupune sfârșitul jocului. Pentru a intra în atmosfera piraților scena 2D va fi acompaniată de coloana sonoră a filmului “Pirates of the Caribbean”.

**Aspecte punctuale**

• Jocul poate fi întrerupt prin apăsarea tastei ESC, iar acest lucru este implementat cu ajutorul funcției glutKeyboardFunc

• Mișcarea corabiei se realizează cu ajutorul funcției glutPassiveMotionFunc ce privește ca parametru un pointer la o funcție. Această funcție controlează pe lângă mișcarea bărcii și un parametru global care va îndrepta corabia în funcție de direcția de deplasare a mouse-ului.

• Scena este desenată astfel încât dimensiunea obiectelor să fie automat scalată indiferent de dimensiunea display-ului. În acest sens s-au utilizat funcțiile glutGet(GLUT_SCREEN_WIDTH) și glutGet(GLUT_SCREEN_HEIGHT), iar pe baza acestor două output-urilor se va calcula un aspect ratio.

• Inițial întreaga scenă 2D este colorată în albastru deschis astfel încât să reprezinte marea.

• Cerul este desenat pe baza unui poligon cu 4 laturi. Dimensiunea cerului este stabilită în mod dinamic în funcție de dimensiunea ecranului.

• Soarele este desenat pe baza unui poligon cu 360 de laturi.

• În scena 2D sunt introduse și 4 insule. Cele 4 insule au dimensiuni diferite pentru a simula faptul că se află la distanțe diferite față de corabie. Fiecare insulă este reprezentată printr-o fâșie de nisip și un palmier. Nisipul este desenat prin intermediul intersecției a două cercuri, dintre care unul de culoarea mării) astfel încât forma insulei va fi reprezentată de un semi-cerc. Dimensiunea elementelor care constituie palmierul se stabilește în mod dinamic în funcție de dimensiunea insulei. Frunzele palmierilor sunt făcute din arce de cerc suprapuse astfel încât să se obțină rezultatul dorit. De asemenea, tot prin suprapunerea cercurilor de aceeași culoare, dar nuanțe diferite se obține și efectul de shadow. Trunchiul palmierului este implementat pe baza GL_QUADS.

• Corabia și componentele sale au fost implementate utilizând următoarele figuri geometrice: poligoane, segmente și triunghiuri. Elementele care compun corabia sunt agregate prin intermediul funcțiilor glPushMatrix și glPopMatrix. De asemenea, funcțiile glTranslated și glRotatef sunt folosite pentru a roti barca în direcția de deplasare a mouse-ului.

• Pe baza implementării distribuției uniforme (numere întregi) din cadrul librăriei random s-a realizat o funcție care va genera numere aleatoare întregi dintr-un anumit interval. Această funcție va fi utilizată pentru genera obiecte random și poziții random pe axa OX.

• Obiectele care trebuie strânse (cufărul, moneda și sticla), dar și ghiuleaua (care trebuie evitată de corabie) sunt implementate cu ajutorul OOP prin intermediul clasei BaseItem. Această clasă conține pozițiile x și y precum și funcțiile virtuale drawItem și droppedItem. Celelalte obiecte vor moșteni această clasă și vor implementa cele 2 funcții virtuale, iar desenarea lor efectivă se realizează pe baza poligoanelor și a quad-urilor. Implementarea obiectelor are de asemenea în vedere mișcarea lor de-a lungul axei OY. Atunci când corabia prinde o sticlă, monedă sau un cufăr scorul este incrementat în funcție de bonusul aferent obiectului prins, iar atunci cănd
corabia de pirați atinge ghiuleaua jocul se termină, iar pe ecran apare mesajul GAME OVER.

• La intervale de o secundă se generează în mod aleator diferite tipuri de obiecte (monedă, sticlă, cufăr sau ghiulea). Obiectele pleacă de la limita superioară a axei OY și de pe poziții aleatoare aferente axei OX. Design-ul acestor obiecte este realizat în așa fel încât să permită mișcarea lor în jos pe axa OY.

• Scorul jocului a fost afișat utilizând funcțiile glRasterPos2f și glutBitmapCharacter.
