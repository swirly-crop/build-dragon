KOTLINC = kotlinc
JAVA = java
TARGET = Main.jar

all: $(TARGET)

$(TARGET): Main.kt
	$(KOTLINC) Main.kt -include-runtime -d $(TARGET)

run: $(TARGET)
	$(JAVA) -jar $(TARGET)

clean:
	rm -f $(TARGET)