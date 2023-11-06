import pandas as pd
import numpy as np
import itertools

word2int = {
    "SELECT": 0,
     "DELETE": 1,
     "*": 2,
     "FROM": 3,
     "db": 4,
     "database": 5,
     "WHERE": 6,
     "=": 7,
     "'": 8,
     "users": 9,
     "passwords": 10,
     ";": 11,
     ",": 12,
     " ": 13,
     "&": 14,
     "LIMIT": 15,
     "admin": 16,
     "100": 17,
     "10": 18,
     "id": 19,
     "fred": 20,
     "eos": 21
}

def get_answer(query):
    answer = []
    for i in range(len(query)):
        answer += [word2int[query[i]]]
    for j in range(len(query),22):
        answer += [21]
    return answer

def shuffle2_query(query):
    queries = []
    for i in range(len(query)-1):
        for j in range(i+1, len(query)):
            queries += [query[:i] + [query[j]] + query[i+1:j] + [query[i]] + query[j+1:]]
    
    return queries

def shuffle3_query(query):
    queries = []
    for i in range(len(query)-2):
        for j in range(i+1, len(query)-1):
            for k in range(j+1,len(query)):
                queries += [query[:i] + [query[k]] + query[i+1:j] + [query[j]] + query[j+1:k] + [query[i]] + query[k+1:]]
    
    return queries

def shuffle4_query(query):
    queries = []
    for i in range(len(query)-3):
        for j in range(i+1, len(query)-2):
            for k in range(j+1,len(query) - 1):
                for l in range(k+1,len(query)):
                    queries += [query[:i] + [query[l]] + query[i+1:j] + [query[k]] + query[j+1:k] + [query[j]] + query[k+1:l] + [query[i]] + query[l+1:]]
    
    return queries



# correct queries to shuffle
queries = [["SELECT"," ", "users", ",", "passwords"," ", "FROM"," ", "db", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords"," ", "FROM"," ", "db", ";"]]
queries += [["SELECT"," ", "users", ",", "id"," ", "FROM"," ", "db", ";"]]
queries += [["DELETE"," ", "users", ",", "id"," ", "FROM"," ", "db", ";"]]
queries += [["SELECT"," ", "id", ",", "passwords"," ", "FROM"," ", "db", ";"]]
queries += [["DELETE"," ", "id", ",", "passwords"," ", "FROM"," ", "db", ";"]]
#queries += [["SELECT"," ", "*"," ", "FROM"," ", "db", ";"]]
#queries += [["DELETE"," ", "*"," ", "FROM"," ", "db", ";"]]

queries += [["SELECT"," ", "users", ",", "passwords"," ", "FROM"," ", "database", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords"," ", "FROM"," ", "database", ";"]]
queries += [["SELECT"," ", "users", ",", "id"," ", "FROM"," ", "database", ";"]]
queries += [["DELETE"," ", "users", ",", "id"," ", "FROM"," ", "database", ";"]]
queries += [["SELECT"," ", "id", ",", "passwords"," ", "FROM"," ", "database", ";"]]
queries += [["DELETE"," ", "id", ",", "passwords"," ", "FROM"," ", "database", ";"]]
#queries += [["SELECT"," ", "*"," ", "FROM"," ", "database", ";"]]
#queries += [["DELETE"," ", "*"," ", "FROM"," ", "database", ";"]]

queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", ";"]]

queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "id", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "users", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords"," ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", ",", "passwords"," ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords"," ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", ",", "passwords"," ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords"," ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users", ",", "passwords", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords"," ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "id", ",", "passwords", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", ",", "passwords"," ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "passwords"," ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", ",", "passwords"," ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "passwords"," ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", ",", "passwords"," ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", ",", "passwords", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", ",", "passwords"," ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "id", ",", "users", " ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", ",", "users", " ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]]


queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", ",", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", ",", "id"," ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id",",", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "10", ";"]]


queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]

queries += [["SELECT"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", ",", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", ",", "id"," ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id",",", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "users", "=","'","admin", "'", " ", "LIMIT", " ", "100", ";"]]

queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]

queries += [["SELECT"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords", ",", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords", ",", "id"," ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]
queries += [["DELETE"," ", "id",",", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "10", ";"]]


queries += [["SELECT"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "*", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]

queries += [["SELECT"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users", ",", "passwords", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords", ",", "id", " ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords", ",", "id"," ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id", ",", "users"," ", "FROM"," ", "database", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "users",",", "passwords", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "passwords",",", "id", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["SELECT"," ", "id", ",", "users"," ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]
queries += [["DELETE"," ", "id",",", "users", " ", "FROM"," ", "db", " ", "WHERE", " ", "id", "=","'","fred", "'", " ", "LIMIT", " ", "100", ";"]]

'''
# queries with syntax errors
query11 = ["SELECT"," ", "&"," ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]
query12 = ["DELETE"," ", "&"," ", "FROM"," ", "db", " ", "LIMIT", " ", "10", ";"]
query13 = ["SELECT","*","FROM","db","LIMIT","10"]
query14 = ["DELETE","*","FROM","db","LIMIT","10"]
query15 = ["SELECT"," ", "users"," ", "WHERE"," ", "database", " ", "LIMIT", " ", "10", ";"]
query16 = ["DELETE"," ", "users"," ", "WHERE"," ", "database", " ", "LIMIT", " ", "10", ";"]
query17 = ["FROM"," ", "database"," ", "SELECT"," ", "passwords", " ", "WHERE", "users", "=","'","admin"]
query18 = ["FROM"," ", "database"," ", "DELETE"," ", "passwords", " ", "WHERE", "users", "=","'","admin"]
query19 = ["SELECT"," ", "db"," ", "FROM"," ", "users", " ", "LIMIT", " ", "10", ";"]
query20 = ["DELETE"," ", "db"," ", "FROM"," ", "users", " ", "LIMIT", " ", "10", ";"]
'''

def shuffle_and_print(query,answer):
    qs2 = shuffle2_query(query)
    qs3 = shuffle3_query(query)
    qs4 = shuffle4_query(query)

    result = list(zip(qs2, [answer for _ in range(len(qs2))]))
    result += list(zip(qs3, [answer for _ in range(len(qs3))]))
    result += list(zip(qs4, [answer for _ in range(len(qs4))]))

    return result

'''
result = shuffle_and_print(query1,query1)
for i in range(len(result)):
    print(result[i])
    print('\n')
'''

results = []

for query in queries:
   # print(query)
    results += [shuffle_and_print(query,query)]

for i in range(len(results[0])):
   for result in results:
       if i < len(result):
            print(result[i])
            print('\n')
    