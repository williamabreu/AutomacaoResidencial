// pinos dos sensores:
#define pinLM35  A0
#define pinLDR   A1
#define pinPIR    2
#define pinRESET  3 

// pinos dos atuadores:
#define pinLuz       8
#define pinMotor     9
#define pinAr       10


// CONSTANTES:
#define temperaturaRef  25.0
#define luminosidadeRef 800


// variáveis que terão valores lidos dos sensores:
float temperatura;
int   luminosidadeExterna;
bool  presenca;

// variáveis que guardarão o estado do sistema:
bool arLigado;
bool luzAcesa;
bool cortinaAberta;
bool reset;


// protótipo de funções para abstração do programa:
float leituraTemperatura(int pin);
int   leituraLuminosidade(int pin);
bool  leituraPresenca(int pin);
void  ligarAr();
void  desligarAr();
void  acenderLuz();
void  apagarLuz();
void  abrirCortina();
void  fecharCortina();
bool  claro(int luminosidade);
bool  escuro(int luminosidade);



void setup() {
    pinMode(pinLM35, INPUT);
    pinMode(pinLDR, INPUT);
    pinMode(pinPIR, INPUT);
    pinMode(pinRESET, INPUT_PULLUP);
    
    pinMode(pinLuz, OUTPUT);
    pinMode(pinMotor, OUTPUT);
    pinMode(pinAr, OUTPUT);
    
    desligarAr();
    apagarLuz();    
    fecharCortina();
    reset = false;
    
    Serial.begin(9600);
}



void loop() {
    presenca = leituraPresenca(pinPIR);
    
    if (presenca) { 
        Serial.println("Presenca detectada!");
        delay(2000);         
        do {
            temperatura = leituraTemperatura(pinLM35);
            Serial.print("Temperatura: ");
            Serial.println(temperatura);
            
            if (temperatura > temperaturaRef) {
                Serial.println("- Ar condicionado: ON");
                ligarAr();
            }
            else {
                Serial.println("- Ar condicionado: OFF");
                desligarAr();
            }

            delay(2000);
            
            luminosidadeExterna = leituraLuminosidade(pinLDR);
            Serial.print("Luminosidade: ");
            Serial.println(luminosidadeExterna);
            
            if ( claro(luminosidadeExterna) and not cortinaAberta ) {
                Serial.println("- Abrindo cortina\n- Luz: OFF");
                abrirCortina();
                apagarLuz();
            }
            else if ( claro(luminosidadeExterna) and cortinaAberta ) {
                Serial.println("- Luz: OFF");
                apagarLuz();
            }
            else if ( escuro(luminosidadeExterna) and not cortinaAberta ) {
                Serial.println("- Luz: ON");
                acenderLuz();
            }
            else { // if ( escuro(luminosidadeExterna) and cortinaAberta )
                Serial.println("- Luz: ON\n- Fechando cortina");
                acenderLuz();
                fecharCortina();
            }

            delay(2000);

            Serial.println();
         
            delay(500);
            reset = not digitalRead(pinRESET);
        
        } while (not reset);

        Serial.println("Apertou reset!");
        desligarAr();
        apagarLuz();    
        fecharCortina();
        reset = false;
    }
    else {
        Serial.println("Ninguem ainda...");
    }

    delay(2000);
    delay(500);
}





// implementação das funções de abstração

float leituraTemperatura(int pin) {
    // https://portal.vidadesilicio.com.br/lm35-medindo-temperatura-com-arduino/
    return analogRead(pin) * 0.4888;
}

int leituraLuminosidade(int pin) {
    // https://portal.vidadesilicio.com.br/sensor-de-luz-com-ldr/
    return analogRead(pin);
}

bool leituraPresenca(int pin) {
    // https://portal.vidadesilicio.com.br/sensor-presenca-movimento-pir-hc-sr501/
    return digitalRead(pin);
}

void ligarAr() {
    digitalWrite(pinAr, HIGH);
    arLigado = true;
}

void desligarAr() {
    digitalWrite(pinAr, LOW);
    arLigado = false;
}

void acenderLuz() {
    digitalWrite(pinLuz, HIGH);
    luzAcesa = true;
}

void apagarLuz() {
    digitalWrite(pinLuz, LOW);
    luzAcesa = false;
}

void abrirCortina() {
    digitalWrite(pinMotor, HIGH);
    cortinaAberta = true;
}

void fecharCortina() {
    digitalWrite(pinMotor, LOW);
    cortinaAberta = false;
}

bool claro(int luminosidade) {
    if (luminosidade >= luminosidadeRef) {
        return true;
    }
    else {
        return false;
    }
}

bool escuro(int luminosidade) {
    return not claro(luminosidade);
}









