# import mysql.connector
# from mysql.connector import Error




# try:
#     connection = mysql.connector.connect(host='localhost',
#                                          database='cwfa',
#                                          user='root',
#                                          password='')
#     if connection.is_connected():
#         db_Info = connection.get_server_info()
#         print("Connected to MySQL Server version ", db_Info)
#         cursor = connection.cursor()
#         cursor.execute("select database();")
#         record = cursor.fetchone()
#         print("You're connected to database: ", record)

# except Error as e:
#     print("Error while connecting to MySQL", e)
# finally:
#     if (connection.is_connected()):
#         cursor.close()
#         connection.close()
#         print("MySQL connection is closed")



words = dict()

f = open('src/lemmatization-es.txt','r').readlines()

print(len(f))

for i in f:
    text = i.replace("\n","")
    row = text.split('\t')

    if row[0] not in words:
        if row[0].isnumeric() == False:
            words[row[0]]=row[0]
    
    if row[1] not in words:
        if row[1].isnumeric() == False:
            words[row[1]]=row[1]


f = open('src/diccionario.csv','a')
for i in words:
    w = i.replace("\n","")
    f.write(w+'\n')
f.close()

print(len(words))