# Folosește imaginea de bază specificată
FROM critoma/linux-u20-dev-security-ism

# Setează variabila de lucru
WORKDIR /app

# Instalează Java 19 folosind AdoptOpenJDK (exemplu de nume de pachet, modifică conform disponibilității)
RUN apt-get update && apt-get install -y openjdk-17-jdk

# Copiază fișierul JAR al aplicației în container
COPY target/ProjectPart2-0.0.1-SNAPSHOT.jar /app/ProjectPart2-0.0.1-SNAPSHOT.jar

# Expune portul pe care rulează aplicația Spring Boot
EXPOSE 8081

# Comanda pentru a rula aplicația
CMD ["java", "-jar", "/app/ProjectPart2-0.0.1-SNAPSHOT.jar"]