class Racket {
private:
	float xPosition;
	float yPosition;
	float width;
	float height;

public:
	void setxPosition(float position) {
		xPosition = position;
	}
	void setyPosition(float position) {
		yPosition = position;
	}
	void setwidth(float width) {
		this->width = width;
	}
	void setheight(float height) {
		this->height = height;
	}
	float getxPosition() {
		return xPosition;
	}
	float getyPositionn() {
		return yPosition;
	}
	float getwidth() {
		return width;
	}
	float getheight() {
		return height;
	}
};