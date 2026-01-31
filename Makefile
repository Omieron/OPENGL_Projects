# OpenGL Projects Makefile for macOS
# Usage: make all    - Compile all projects
#        make run1   - Run Arrow&Shadow
#        make run2   - Run Ballon&Arrow
#        make run3   - Run SpaceInvade
#        make run4   - Run TankvsHelicopter
#        make clean  - Clean all binaries

CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wno-deprecated-declarations
FRAMEWORKS = -framework OpenGL -framework GLUT

# Output binaries
BIN1 = arrow_shadow
BIN2 = ballon_arrow
BIN3 = space_invade
BIN4 = tank_helicopter

.PHONY: all clean run1 run2 run3 run4

all: $(BIN1) $(BIN2) $(BIN3) $(BIN4)
	@echo ""
	@echo "✅ Tüm projeler başarıyla derlendi!"
	@echo ""
	@echo "Çalıştırmak için:"
	@echo "  make run1  - Arrow&Shadow"
	@echo "  make run2  - Ballon&Arrow"
	@echo "  make run3  - SpaceInvade"
	@echo "  make run4  - TankvsHelicopter"
	@echo ""

$(BIN1):
	@echo "🔨 Derleniyor: Arrow&Shadow..."
	$(CXX) $(CXXFLAGS) -I"Arrow&Shadow" "Arrow&Shadow/Source.cpp" -o $@ $(FRAMEWORKS)
	@echo "✅ $(BIN1) hazır"

$(BIN2):
	@echo "🔨 Derleniyor: Ballon&Arrow..."
	$(CXX) $(CXXFLAGS) -I"Ballon&Arrow" "Ballon&Arrow/Source.cpp" -o $@ $(FRAMEWORKS)
	@echo "✅ $(BIN2) hazır"

$(BIN3):
	@echo "🔨 Derleniyor: SpaceInvade..."
	$(CXX) $(CXXFLAGS) -I"SpaceInvade" "SpaceInvade/Source.cpp" -o $@ $(FRAMEWORKS)
	@echo "✅ $(BIN3) hazır"

$(BIN4):
	@echo "🔨 Derleniyor: TankvsHelicopter..."
	$(CXX) $(CXXFLAGS) -I"TankvsHelicopter" "TankvsHelicopter/Source.cpp" -o $@ $(FRAMEWORKS)
	@echo "✅ $(BIN4) hazır"

run1: $(BIN1)
	@echo "🚀 Arrow&Shadow başlatılıyor..."
	./$(BIN1)

run2: $(BIN2)
	@echo "🚀 Ballon&Arrow başlatılıyor..."
	./$(BIN2)

run3: $(BIN3)
	@echo "🚀 SpaceInvade başlatılıyor..."
	./$(BIN3)

run4: $(BIN4)
	@echo "🚀 TankvsHelicopter başlatılıyor..."
	./$(BIN4)

clean:
	@echo "🧹 Temizleniyor..."
	rm -f $(BIN1) $(BIN2) $(BIN3) $(BIN4)
	@echo "✅ Temizlendi"
