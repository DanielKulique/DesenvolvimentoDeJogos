#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace espaco_a {

int fator = 10;

int calcular(int valor) {
    return valor + fator;
}

int dobrar(int valor) {
    return valor * 2;
}

namespace interno {

int calcular(int valor) {
    return valor - 1;
}

}

}

namespace espaco_b {

int fator = 20;

int calcular(int valor) {
    return valor * fator;
}

}

namespace {

int chamadas_registradas = 0;

void registrar_chamada() {
    ++chamadas_registradas;
}

}

template <typename T>
class ListaEncadeada {
public:
    ListaEncadeada() : cabeca_(nullptr), tamanho_(0) {}

    ListaEncadeada(const ListaEncadeada&) = delete;
    ListaEncadeada& operator=(const ListaEncadeada&) = delete;

    ~ListaEncadeada() {
        limpar();
    }

    void inserir_no_inicio(const T& valor) {
        cabeca_ = new No{valor, cabeca_};
        ++tamanho_;
    }

    void inserir_no_fim(const T& valor) {
        No* novo = new No{valor, nullptr};
        if (cabeca_ == nullptr) {
            cabeca_ = novo;
        } else {
            No* ultimo = cabeca_;
            while (ultimo->proximo != nullptr) {
                ultimo = ultimo->proximo;
            }
            ultimo->proximo = novo;
        }
        ++tamanho_;
    }

    bool remover(const T& valor) {
        No* anterior = nullptr;
        No* atual = cabeca_;
        while (atual != nullptr && !(atual->valor == valor)) {
            anterior = atual;
            atual = atual->proximo;
        }
        if (atual == nullptr) {
            return false;
        }
        if (anterior == nullptr) {
            cabeca_ = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        delete atual;
        --tamanho_;
        return true;
    }

    bool contem(const T& valor) const {
        for (const No* atual = cabeca_; atual != nullptr; atual = atual->proximo) {
            if (atual->valor == valor) {
                return true;
            }
        }
        return false;
    }

    std::size_t tamanho() const {
        return tamanho_;
    }

    void imprimir() const {
        if (cabeca_ == nullptr) {
            std::cout << "(vazia)\n";
            return;
        }
        for (const No* atual = cabeca_; atual != nullptr; atual = atual->proximo) {
            std::cout << atual->valor;
            if (atual->proximo != nullptr) {
                std::cout << " -> ";
            }
        }
        std::cout << '\n';
    }

    void limpar() {
        while (cabeca_ != nullptr) {
            No* proximo = cabeca_->proximo;
            delete cabeca_;
            cabeca_ = proximo;
        }
        tamanho_ = 0;
    }

private:
    struct No {
        T valor;
        No* proximo;
    };

    No* cabeca_;
    std::size_t tamanho_;
};

struct Ponto {
    double x;
    double y;
};

double distancia(const Ponto& a, const Ponto& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

class Retangulo {
public:
    Retangulo(double largura, double altura) : largura_(largura), altura_(altura) {}

    double largura() const {
        return largura_;
    }

    double altura() const {
        return altura_;
    }

    double area() const {
        return largura_ * altura_;
    }

    double perimetro() const {
        return 2.0 * (largura_ + altura_);
    }

    void redimensionar(double fator) {
        largura_ *= fator;
        altura_ *= fator;
    }

private:
    double largura_;
    double altura_;
};

class Motor {
public:
    explicit Motor(int potencia) : potencia_(potencia), ligado_(false) {}

    void ligar() {
        ligado_ = true;
    }

    void desligar() {
        ligado_ = false;
    }

    bool ligado() const {
        return ligado_;
    }

    int potencia() const {
        return potencia_;
    }

private:
    int potencia_;
    bool ligado_;
};

class Carro {
public:
    Carro(const std::string& modelo, int potencia) : modelo_(modelo), motor_(potencia) {}

    const std::string& modelo() const {
        return modelo_;
    }

    Motor& motor() {
        return motor_;
    }

    const Motor& motor() const {
        return motor_;
    }

private:
    std::string modelo_;
    Motor motor_;
};

class Garagem {
public:
    explicit Garagem(const std::string& nome) : nome_(nome) {}

    void estacionar(Carro* carro) {
        carros_.push_back(carro);
    }

    bool transferir(const std::string& modelo, Garagem& destino) {
        std::vector<Carro*>::iterator posicao = std::find_if(
            carros_.begin(), carros_.end(),
            [&modelo](const Carro* carro) { return carro->modelo() == modelo; });
        if (posicao == carros_.end()) {
            return false;
        }
        destino.estacionar(*posicao);
        carros_.erase(posicao);
        return true;
    }

    void listar() const {
        std::cout << nome_ << ":";
        if (carros_.empty()) {
            std::cout << " (vazia)";
        }
        for (const Carro* carro : carros_) {
            std::cout << ' ' << carro->modelo();
        }
        std::cout << '\n';
    }

private:
    std::string nome_;
    std::vector<Carro*> carros_;
};

class Recurso {
public:
    explicit Recurso(const std::string& nome) : nome_(nome) {
        std::cout << "  [" << nome_ << "] criado\n";
    }

    Recurso(const Recurso&) = delete;
    Recurso& operator=(const Recurso&) = delete;

    ~Recurso() {
        std::cout << "  [" << nome_ << "] destruído\n";
    }

    const std::string& nome() const {
        return nome_;
    }

private:
    std::string nome_;
};

class Vetor2D {
public:
    Vetor2D(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}

    double x() const {
        return x_;
    }

    double y() const {
        return y_;
    }

    Vetor2D operator+(const Vetor2D& outro) const {
        return Vetor2D(x_ + outro.x_, y_ + outro.y_);
    }

    Vetor2D operator-(const Vetor2D& outro) const {
        return Vetor2D(x_ - outro.x_, y_ - outro.y_);
    }

    Vetor2D operator-() const {
        return Vetor2D(-x_, -y_);
    }

    Vetor2D operator*(double escalar) const {
        return Vetor2D(x_ * escalar, y_ * escalar);
    }

    Vetor2D& operator+=(const Vetor2D& outro) {
        x_ += outro.x_;
        y_ += outro.y_;
        return *this;
    }

    bool operator==(const Vetor2D& outro) const {
        return x_ == outro.x_ && y_ == outro.y_;
    }

    bool operator!=(const Vetor2D& outro) const {
        return !(*this == outro);
    }

    double modulo() const {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    friend Vetor2D operator*(double escalar, const Vetor2D& vetor) {
        return vetor * escalar;
    }

    friend std::ostream& operator<<(std::ostream& saida, const Vetor2D& vetor) {
        return saida << '(' << vetor.x_ << ", " << vetor.y_ << ')';
    }

private:
    double x_;
    double y_;
};

class SaldoInsuficiente : public std::runtime_error {
public:
    SaldoInsuficiente(double saldo, double pedido)
        : std::runtime_error("saldo insuficiente"), saldo_(saldo), pedido_(pedido) {}

    double saldo() const {
        return saldo_;
    }

    double pedido() const {
        return pedido_;
    }

private:
    double saldo_;
    double pedido_;
};

class ContaBancaria {
public:
    explicit ContaBancaria(double saldo_inicial) : saldo_(saldo_inicial) {
        if (saldo_inicial < 0.0) {
            throw std::invalid_argument("saldo inicial negativo");
        }
    }

    void depositar(double valor) {
        if (valor <= 0.0) {
            throw std::invalid_argument("valor de depósito inválido");
        }
        saldo_ += valor;
    }

    void sacar(double valor) {
        if (valor > saldo_) {
            throw SaldoInsuficiente(saldo_, valor);
        }
        saldo_ -= valor;
    }

    double saldo() const noexcept {
        return saldo_;
    }

private:
    double saldo_;
};

template <typename T>
void imprimir_vetor(const std::string& rotulo, const std::vector<T>& valores) {
    std::cout << rotulo << ":";
    for (const T& valor : valores) {
        std::cout << ' ' << valor;
    }
    std::cout << '\n';
}

double media(const std::vector<double>& valores) {
    assert(!valores.empty());
    return std::accumulate(valores.begin(), valores.end(), 0.0) / valores.size();
}

void operacao_com_falha() {
    Recurso recurso("temporário");
    throw std::runtime_error("falha durante a operação");
}

void titulo(const std::string& texto) {
    std::cout << "\n=== " << texto << " ===\n";
}

void demonstrar_lista_encadeada() {
    std::cout << "Olá mundo\n";

    ListaEncadeada<int> numeros;
    numeros.inserir_no_fim(10);
    numeros.inserir_no_fim(20);
    numeros.inserir_no_fim(30);
    numeros.inserir_no_inicio(5);
    numeros.imprimir();

    std::cout << std::boolalpha;
    std::cout << "contém 20: " << numeros.contem(20) << '\n';
    std::cout << "remover 20: " << numeros.remover(20) << '\n';
    std::cout << "remover 99: " << numeros.remover(99) << '\n';
    numeros.imprimir();
    std::cout << "tamanho: " << numeros.tamanho() << '\n';

    ListaEncadeada<std::string> palavras;
    palavras.inserir_no_fim("alocação");
    palavras.inserir_no_fim("dinâmica");
    palavras.inserir_no_inicio("memória");
    palavras.imprimir();
}

void demonstrar_structs_e_classes() {
    Ponto origem{0.0, 0.0};
    Ponto destino{3.0, 4.0};
    std::cout << "distância entre pontos: " << distancia(origem, destino) << '\n';

    Retangulo retangulo(4.0, 2.5);
    std::cout << "área: " << retangulo.area() << ", perímetro: " << retangulo.perimetro() << '\n';

    retangulo.redimensionar(2.0);
    std::cout << "após redimensionar: " << retangulo.largura() << " x " << retangulo.altura() << '\n';
}

void demonstrar_composicao_e_agregacao() {
    Carro sedan("sedan", 110);
    Carro hatch("hatch", 75);
    Carro utilitario("utilitário", 140);

    sedan.motor().ligar();
    std::cout << "motor do sedan ligado: " << std::boolalpha << sedan.motor().ligado() << '\n';
    std::cout << "motor do hatch ligado: " << hatch.motor().ligado() << '\n';

    Garagem norte("norte");
    Garagem sul("sul");
    norte.estacionar(&sedan);
    norte.estacionar(&hatch);
    sul.estacionar(&utilitario);
    norte.listar();
    sul.listar();

    std::cout << "transferir hatch: " << norte.transferir("hatch", sul) << '\n';
    std::cout << "transferir moto: " << norte.transferir("moto", sul) << '\n';
    norte.listar();
    sul.listar();

    {
        Garagem temporaria("temporária");
        temporaria.estacionar(&sedan);
        temporaria.listar();
    }
    std::cout << "sedan continua existindo: " << sedan.modelo() << " com " << sedan.motor().potencia() << " cv\n";
}

void demonstrar_ponteiros_inteligentes() {
    std::cout << "-- unique_ptr --\n";
    {
        std::unique_ptr<Recurso> unico = std::make_unique<Recurso>("único");
        std::unique_ptr<Recurso> novo_dono = std::move(unico);
        std::cout << "original vazio após mover: " << std::boolalpha << (unico == nullptr) << '\n';
        std::cout << "novo dono: " << novo_dono->nome() << '\n';
    }

    std::vector<std::unique_ptr<Recurso>> recursos;
    recursos.push_back(std::make_unique<Recurso>("a"));
    recursos.push_back(std::make_unique<Recurso>("b"));
    std::cout << "recursos no vetor: " << recursos.size() << '\n';
    recursos.clear();

    std::cout << "-- shared_ptr --\n";
    std::shared_ptr<Recurso> compartilhado = std::make_shared<Recurso>("compartilhado");
    std::cout << "contagem: " << compartilhado.use_count() << '\n';
    {
        std::shared_ptr<Recurso> copia = compartilhado;
        std::cout << "contagem com cópia: " << compartilhado.use_count() << '\n';
    }
    std::cout << "contagem após escopo: " << compartilhado.use_count() << '\n';

    std::weak_ptr<Recurso> observador = compartilhado;
    std::cout << "observador expirado: " << observador.expired() << '\n';
    compartilhado.reset();
    std::cout << "observador expirado após reset: " << observador.expired() << '\n';
}

void demonstrar_stl() {
    std::vector<int> valores = {42, 7, 19, 3, 25};
    valores.push_back(11);
    valores.emplace_back(8);
    imprimir_vetor("original", valores);

    std::sort(valores.begin(), valores.end());
    imprimir_vetor("ordenado", valores);

    std::vector<int>::iterator maior_que_vinte = std::find_if(valores.begin(), valores.end(), [](int v) { return v > 20; });
    if (maior_que_vinte != valores.end()) {
        std::cout << "primeiro maior que 20: " << *maior_que_vinte << '\n';
    }

    std::cout << "soma: " << std::accumulate(valores.begin(), valores.end(), 0) << '\n';
    std::cout << "menor: " << *std::min_element(valores.begin(), valores.end()) << '\n';
    std::cout << "maior: " << *std::max_element(valores.begin(), valores.end()) << '\n';
    std::cout << "pares: " << std::count_if(valores.begin(), valores.end(), [](int v) { return v % 2 == 0; }) << '\n';

    std::vector<int> dobrados(valores.size());
    std::transform(valores.begin(), valores.end(), dobrados.begin(), [](int v) { return v * 2; });
    imprimir_vetor("dobrados", dobrados);

    valores.erase(std::remove_if(valores.begin(), valores.end(), [](int v) { return v % 2 != 0; }), valores.end());
    imprimir_vetor("somente pares", valores);

    std::vector<std::string> palavras = {"banana", "kiwi", "abacaxi", "uva"};
    std::sort(palavras.begin(), palavras.end(), [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
    imprimir_vetor("por tamanho", palavras);
}

void demonstrar_operadores() {
    Vetor2D a(1.0, 2.0);
    Vetor2D b(3.0, -1.0);

    std::cout << "a = " << a << ", b = " << b << '\n';
    std::cout << "a + b = " << a + b << '\n';
    std::cout << "a - b = " << a - b << '\n';
    std::cout << "-a = " << -a << '\n';
    std::cout << "a * 3 = " << a * 3.0 << '\n';
    std::cout << "2 * b = " << 2.0 * b << '\n';

    Vetor2D acumulado;
    acumulado += a;
    acumulado += b;
    std::cout << "acumulado = " << acumulado << '\n';

    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << '\n';
    std::cout << "a != b: " << (a != b) << '\n';
    std::cout << "módulo de (3, 4): " << Vetor2D(3.0, 4.0).modulo() << '\n';
}

void demonstrar_namespaces() {
    std::cout << "espaco_a::calcular(5) = " << espaco_a::calcular(5) << '\n';
    std::cout << "espaco_b::calcular(5) = " << espaco_b::calcular(5) << '\n';
    std::cout << "espaco_a::fator = " << espaco_a::fator << ", espaco_b::fator = " << espaco_b::fator << '\n';
    std::cout << "espaco_a::dobrar(5) = " << espaco_a::dobrar(5) << '\n';

    std::cout << "espaco_a::interno::calcular(5) = " << espaco_a::interno::calcular(5) << '\n';

    namespace interno = espaco_a::interno;
    std::cout << "interno::calcular(8) com alias = " << interno::calcular(8) << '\n';

    using espaco_b::calcular;
    std::cout << "calcular(3) com using = " << calcular(3) << '\n';

    registrar_chamada();
    registrar_chamada();
    std::cout << "chamadas registradas no namespace anônimo: " << chamadas_registradas << '\n';
}

void demonstrar_assertions() {
    static_assert(sizeof(int) >= 4, "int precisa ter ao menos 4 bytes");
    static_assert(sizeof(char) == 1, "char precisa ter 1 byte");

    std::vector<double> notas = {7.0, 8.5, 9.0, 6.5};
    double resultado = media(notas);
    assert(resultado > 0.0);
    assert(std::abs(resultado - 7.75) < 1e-9);

    ListaEncadeada<int> lista;
    assert(lista.tamanho() == 0);
    lista.inserir_no_fim(1);
    lista.inserir_no_fim(2);
    assert(lista.tamanho() == 2);
    assert(lista.contem(2));
    assert(!lista.contem(3));

    std::cout << "média das notas: " << resultado << '\n';
    std::cout << "todas as assertions passaram\n";
}

void demonstrar_excecoes() {
    ContaBancaria conta(100.0);

    try {
        conta.depositar(50.0);
        conta.sacar(30.0);
        std::cout << "saldo: " << conta.saldo() << '\n';
        conta.sacar(500.0);
        std::cout << "esta linha não é executada\n";
    } catch (const SaldoInsuficiente& erro) {
        std::cout << "erro: " << erro.what() << " (saldo " << erro.saldo() << ", pedido " << erro.pedido() << ")\n";
    }

    try {
        conta.depositar(-10.0);
    } catch (const std::invalid_argument& erro) {
        std::cout << "argumento inválido: " << erro.what() << '\n';
    }

    try {
        ContaBancaria invalida(-1.0);
    } catch (const std::exception& erro) {
        std::cout << "falha na construção: " << erro.what() << '\n';
    }

    try {
        std::vector<int> valores = {1, 2, 3};
        int quarto = valores.at(3);
        std::cout << quarto << '\n';
    } catch (const std::out_of_range& erro) {
        std::cout << "fora do intervalo: " << erro.what() << '\n';
    }

    std::cout << "-- liberação de recursos durante a exceção --\n";
    try {
        operacao_com_falha();
    } catch (const std::exception& erro) {
        std::cout << "capturado: " << erro.what() << '\n';
    }

    try {
        try {
            conta.sacar(1000.0);
        } catch (const std::exception& erro) {
            std::cout << "capturado e relançado: " << erro.what() << '\n';
            throw;
        }
    } catch (...) {
        std::cout << "capturado pelo handler genérico\n";
    }
}

int main() {
    titulo("Lista ligada com alocação dinâmica");
    demonstrar_lista_encadeada();

    titulo("Estruturas e classes");
    demonstrar_structs_e_classes();

    titulo("Composição e agregação");
    demonstrar_composicao_e_agregacao();

    titulo("Ponteiros inteligentes");
    demonstrar_ponteiros_inteligentes();

    titulo("Biblioteca STL");
    demonstrar_stl();

    titulo("Sobrecarga de operadores");
    demonstrar_operadores();

    titulo("Espaços de nomes");
    demonstrar_namespaces();

    titulo("Assertions");
    demonstrar_assertions();

    titulo("Tratamento de exceções");
    demonstrar_excecoes();

    return 0;
}