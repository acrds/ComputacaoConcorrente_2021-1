/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr;  
  
  // Construtor
  LE() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      //if (this.escr > 0) {
        while (this.escr > 0) {
        System.out.println ("LeitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
     System.out.println ("LeitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
    System.out.println ("LeitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
       // if ((this.leit > 0) || (this.escr > 0)) {
      while ((this.leit > 0) || (this.escr > 0)) {
         System.out.println ("EscritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println ("EscritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     //System.out.println ("le.escritorSaindo("+id+")");
  }
}


//Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
  int id; //identificador da thread
  //int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Leitor (int id, LE monitor) {
    this.id = id;
    //this.delay = delayTime;
    this.monitor = monitor;
  }

  static boolean valorPrimo(int valor) {
    if (valor<=1){
        return false;
    } 
    for (int i=2; i<=Math.sqrt(valor); i++) {
        if ((float)valor/i%1==0){
            return false;
        } 
    }
    return true;
}

  // Método executado pela thread
    public void run () {
        while (true){
            this.monitor.EntraLeitor(this.id);
            if (valorPrimo(ativ.valor)) {
                System.out.printf("--------------------%d é um valor é primo\n", ativ.valor);
            } else {
                System.out.printf("--------------------%d não é um valor primo\n", ativ.valor);
            }
            this.monitor.SaiLeitor(this.id);
            try {
                sleep(2100);
        } catch (InterruptedException e) { 
        }
        }
    }
}

//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
  int id; //identificador da thread
 // int delay; //atraso bobo...
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritor (int id, LE monitor) {
    this.id = id;
   // this.delay = delayTime;
    this.monitor = monitor;
  }

  // Método executado pela thread
    public void run () {
        while (true) {
            this.monitor.EntraEscritor(this.id);
            ativ.valor = this.id;
            System.out.printf("--------------------Foi escrito %d pelo escritor %d\n", ativ.valor, this.id);
            this.monitor.SaiEscritor(this.id);
            try {
                sleep(2100);
            } catch (InterruptedException e) { 
            }
        }
    }
}

class Leitor_Escritor extends Thread{
    final int id;
    LE monitor;
    public Leitor_Escritor(int id, LE monitor) {
        this.monitor = monitor;
        this.id = id;
    }
    public void run () {
        while (true) {
            this.monitor.EntraLeitor(this.id);
            if (ativ.valor%2==0){
                System.out.printf("--------------------%d é um valor par\n",ativ.valor);
            }else {
                System.out.printf("--------------------%d é um valor ímpar\n",ativ.valor);
            }
            this.monitor.SaiLeitor(this.id);
            this.monitor.EntraEscritor(this.id);
            ativ.valor=ativ.valor*2;
            System.out.printf("--------------------Modificando variavel. Novo valor é %d\n",ativ.valor);
            this.monitor.SaiEscritor(this.id);
            try {
                sleep(2100);
            } catch (InterruptedException e) { 
            }
        }
    }
}

//--------------------------------------------------------
// Classe principal

public class ativ {
    static int valor = 0;
    static final int LetEsc = 3;
    static final int L = 6;
    static final int E = 3;
    public static void main (String[] args) {
        int i;
        Leitor_Escritor[] le = new Leitor_Escritor[LetEsc];  // threads de leitores e escritores
        LE monitor = new LE();//objeto compartilhado 
        Leitor[] l = new Leitor[L]; // thread de leitores
        Escritor[] e = new Escritor[E];// thread de escritores
        for (i=0;i<L;i++){
            l[i]=new Leitor(i+1,monitor);
            l[i].start();
        }
        for (i=0;i<E;i++) {
            e[i]=new Escritor(i+1,monitor);
            e[i].start();
        }
        for (i=0;i<LetEsc;i++) {
            le[i]=new Leitor_Escritor(i+1,monitor);
            le[i].start();
        }
    }
}