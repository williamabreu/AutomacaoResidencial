//#include <Servo.h>

// pinos dos sensores:
#define pinLM35   A0
#define pinLDR    A1
#define pinPIR     2
#define pinRESET   3 

// pinos dos atuadores:
#define pinLuz       4  // LED BRANCO
#define pinMotor     5  // LED VERDE
#define pinAr        6  // LED VERMELHO


// CONSTANTES:
#define temperaturaRef  25.0
#define luminosidadeRef 800


// variáveis que terão valores lidos dos sensores:
float temperatura;
int   luminosidadeExterna;
bool  presenca;

// variáveis que guardarão o estado do sistema:
bool arLigado = false;
bool luzAcesa = false;
bool cortinaAberta = false;
bool reset = false;


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
void  restar();
bool  quente(float temperatura);
bool  claro(int luminosidade);



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
}



void loop() {
    presenca = leituraPresenca(pinPIR);
    
    if (presenca) { 
        do {
            temperatura = leituraTemperatura(pinLM35);
            bool estaQuente = quente(temperatura);
            
            if (estaQuente) {
                ligarAr();
            }
            else {
                desligarAr();
            }
            
            luminosidadeExterna = leituraLuminosidade(pinLDR);
            bool estaClaro = claro(luminosidadeExterna);
            
            if (estaClaro and not cortinaAberta) {
                abrirCortina();
                apagarLuz();
            }
            else if (estaClaro and cortinaAberta) {
                apagarLuz();
            }
            else if (not estaClaro and not cortinaAberta) {
                acenderLuz();
            }
            else { 
                acenderLuz();
                fecharCortina();
            }
            
            reset = not digitalRead(pinRESET);
        
        } while (not reset);

        resetar();
        reset = false;
    }
}



// implementação das funções de abstração

void resetar() {
    if (arLigado) {
        desligarAr();
    }
    if (luzAcesa) {
        apagarLuz();    
    }
    if (cortinaAberta) {
        fecharCortina();
    }
}

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
    for (int i = 0; i < 10; i++) {
        delay(100);
        digitalWrite(pinMotor, LOW);
        delay(100);
        digitalWrite(pinMotor, HIGH);
    }
    
    cortinaAberta = true;
}

void fecharCortina() {
    for (int i = 0; i < 10; i++) {
        delay(100);
        digitalWrite(pinMotor, HIGH);
        delay(100);
        digitalWrite(pinMotor, LOW);
    }
    
    cortinaAberta = false;
}

bool quente(float temperatura) {
    if (temperatura >= temperaturaRef) {
        return true;
    }
    else {
        return false;
    }
}

bool claro(int luminosidade) {
    if (luminosidade >= luminosidadeRef) {
        return true;
    }
    else {
        return false;
    }
}









