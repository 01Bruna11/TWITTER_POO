#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>

class Medico {
    std::string id;
    std::string especialidade;
    std::map<std::string, std::string> pacientes;
public:
    Medico(std::string id, std::string especialidade) : id(id), especialidade(especialidade) {}
    void addPaciente(std::string id, std::string diagnostico) {
        pacientes[id] = diagnostico;
    }
    std::string getEspecialidade() {
        return especialidade;
    }
    std::string getId() {
        return id;
    }
    std::map<std::string, std::string> getPacientes() {
        return pacientes;
    }
    void addPaciente(std::string id, std::string diagnostico) {
        if (pacientes.find(id) == pacientes.end()) {
            pacientes[id] = diagnostico;
        }
    }
    void removePaciente(std::string id) {
        pacientes.erase(id);
    }
    void getPacientes(std::function<void(std::string, std::string)> fn) {
        for (auto& item : pacientes) {
            fn(item.first, item.second);
        }
    }
    std::string toString() {
        std::stringstream ss;
        ss << "Medico: " << id << " Especialidade: " << especialidade << " Pacientes: ";
        for (auto& item : pacientes) {
            ss << item.first << " ";
        }
        return ss.str();
    }
};

class Paciente {
    std::string id; //id = nome
    std::string diagnostico;
    std::map<std::string, std::shared_ptr<Medico>> medicos;
public:
    Paciente(std::string id, std::string diagnostico) : id(id), diagnostico(diagnostico) {}
    void addMedico(std::string id, std::shared_ptr<Medico> medico) {
        medicos[id] = medico;
    }
    std::string getId() {
        return id;
    }
    std::string getDiagnostico() {
        return diagnostico;
    }
    std::map<std::string, std::shared_ptr<Medico>> getMedicos() {
        return medicos;
    }
    void addMedico(std::string id, std::shared_ptr<Medico> medico) {
        if (medicos.find(id) == medicos.end()) {
            medicos[id] = medico;
        }
    }
    void removeMedico(std::string id) {
        medicos.erase(id);
    }  
    std::string toString() {
        std::stringstream ss;
        ss << "Paciente: " << id << " Diagnostico: " << diagnostico << " Medicos: ";
        for (auto& medico : medicos)
            ss << medico.first << " ";
        return ss.str();
    }
};   

class Hospital {
    std::map<std::string, std::shared_ptr<Paciente>> pacientes;
    std::map<std::string, std::shared_ptr<Medico>> medicos;
public:
    void addPaciente(std::string id, std::string diagnostico) {
        if (pacientes.find(id) == pacientes.end()) {
            pacientes[id] = std::make_shared<Paciente>(id, diagnostico);
        }
    }
    void addMedico(std::string id, std::string especialidade) {
        if (medicos.find(id) == medicos.end()) {
            medicos[id] = std::make_shared<Medico>(id, especialidade);
        }
    }
    void addPaciente(std::string id, std::string diagnostico, std::string idMedico, std::string especialidade) {
        if (pacientes.find(id) == pacientes.end()) {
            pacientes[id] = std::make_shared<Paciente>(id, diagnostico);
        }
        if (medicos.find(idMedico) == medicos.end()) {
            medicos[idMedico] = std::make_shared<Medico>(idMedico, especialidade);
        }
        pacientes[id]->addMedico(idMedico, medicos[idMedico]);
        medicos[idMedico]->addPaciente(id, diagnostico);
    }
    void removePaciente(std::string id) {
        if (pacientes.find(id) != pacientes.end()) {
            for (auto& medico : pacientes[id]->getMedicos()) {
                medico.second->removePaciente(id);
            }
            pacientes.erase(id);
        }
    }
    void removeMedico(std::string id) {
        if (medicos.find(id) != medicos.end()) {
            for (auto& paciente : medicos[id]->getPacientes()) {
                paciente.second->removeMedico(id);
            }
            medicos.erase(id);
        }
    }
    //Retorna uma lista de pacientes que são atendidos por um médico.
    std::vector<std::string> getPacientes(std::string idMedico) {
        std::vector<std::string> pacientes;
        if (medicos.find(idMedico) != medicos.end()) {
            for (auto& paciente : medicos[idMedico]->getPacientes()) {
                pacientes.push_back(paciente.first);
            }
        }
        return pacientes;
    }
    //Retorna uma lista de médicos que atendem um paciente.
    std::vector<std::string> getMedicos(std::string idPaciente) {
        std::vector<std::string> medicos;
        if (pacientes.find(idPaciente) != pacientes.end()) {
            for (auto& medico : pacientes[idPaciente]->getMedicos()) {
                medicos.push_back(medico.first);
            }
        }
        return medicos;
    }
    std::string toString() {
        std::stringstream ss;
        ss << "Pacientes: ";
        for (auto& paciente : pacientes) {
            ss << paciente.second->toString() << "\n";
        }
        ss << "Medicos: ";
        for (auto& medico : medicos) {
            ss << medico.second->toString() << "\n";
        }
        return ss.str();
    }
};

int main () {
    Hospital hospital;
    while(true) {
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "addPaciente") {
            std::string id, diagnostico;
            ss >> id >> diagnostico;
            hospital.addPaciente(id, diagnostico);
        } else if (cmd == "addMedico") {
            std::string id, especialidade;
            ss >> id >> especialidade;
            hospital.addMedico(id, especialidade);
        } else if (cmd == "addPaciente") {
            std::string id, diagnostico, idMedico, especialidade;
            ss >> id >> diagnostico >> idMedico >> especialidade;
            hospital.addPaciente(id, diagnostico, idMedico, especialidade);
        } else if (cmd == "removePaciente") {
            std::string id;
            ss >> id;
            hospital.removePaciente(id);
        } else if (cmd == "removeMedico") {
            std::string id;
            ss >> id;
            hospital.removeMedico(id);
        } else if (cmd == "getPacientes") {
            std::string idMedico;
            ss >> idMedico;
            std::vector<std::string> pacientes = hospital.getPacientes(idMedico);
            std::cout << "Pacientes: ";
            for (auto& paciente : pacientes) {
                std::cout << paciente << " ";
            }
            std::cout << "\n";
        } else if (cmd == "getMedicos") {
            std::string idPaciente;
            ss >> idPaciente;
            std::vector<std::string> medicos = hospital.getMedicos(idPaciente);
            std::cout << "Medicos: ";
            for (auto& medico : medicos) {
                std::cout << medico << " ";
            }
            std::cout << "\n";
        } else if (cmd == "toString") {
            std::cout << hospital.toString() << "\n";
        } else if (cmd == "exit") {
            break;
        }
    }
    return 0;
}