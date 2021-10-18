#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	this->font_size = 50;
	this->font.loadFont("fonts/msgothic.ttc", this->font_size, true, true, true);

	this->charactors = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int x = -300; x <= 300; x += 60) {

		for (int y = -300; y <= 300; y += 60) {

			float noise_value = ofMap(ofNoise(x * 0.005, y * 0.005, ofGetFrameNum() * 0.00015), 0, 1, 0, this->charactors.size());
			int index = noise_value;
			int next_index = (index + 1) % this->charactors.size();
			float progress = (noise_value - index) * 100;

			if (progress < 30) {
				
				progress = 0;
			}
			else if (progress < 70) {

				progress = ofMap(progress, 30, 70, 0, 100);
			}
			else {

				progress = 100;
			}

			this->draw_blend_charactor(glm::vec3(x, y, 0), this->charactors[index], this->charactors[next_index], progress);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw_blend_charactor(glm::vec3 location, char c1, char c2, float progress) {

	int total_sample_count = 80;

	ofPushMatrix();
	ofTranslate(location);

	ofPath path = this->font.getCharacterAsPoints(c1, true, false);
	vector<ofPolyline> outline = path.getOutline();
	int sample_count = total_sample_count / outline.size();
	vector<glm::vec3> vertices_1;
	for (int outline_index = 0; outline_index < (int)outline.size(); outline_index++) {

		outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
		vector<glm::vec3> vertices = outline[outline_index].getVertices();
		for (size_t vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

			vertices_1.push_back(vertices[vertices_index] + glm::vec3(this->font_size * -0.5, this->font_size * 0.5, 0));
		}
	}

	ofPath next_path = this->font.getCharacterAsPoints(c2, true, false);
	vector<ofPolyline> next_outline = next_path.getOutline();
	int next_sample_count = total_sample_count / next_outline.size();
	vector<glm::vec3> vertices_2;
	for (int outline_index = 0; outline_index < (int)next_outline.size(); outline_index++) {

		next_outline[outline_index] = next_outline[outline_index].getResampledByCount(next_sample_count);
		vector<glm::vec3> vertices = next_outline[outline_index].getVertices();
		for (size_t vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

			vertices_2.push_back(vertices[vertices_index] + glm::vec3(this->font_size * -0.5, this->font_size * 0.5, 0));
		}
	}

	vector<glm::vec3> draw_vertices;
	for (int i = 0; i < total_sample_count; i++) {

		if (i < vertices_1.size() && i < vertices_2.size()) {

			glm::vec3 p = (vertices_2[i] - vertices_1[i]) * progress * 0.01 + vertices_1[i];
			draw_vertices.push_back(p);
		}
	}

	ofFill();
	for (int i = 0; i < draw_vertices.size(); i++) {

		ofDrawCircle(draw_vertices[i], 2);
	}

	
	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}