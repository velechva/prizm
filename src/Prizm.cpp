#include "plugin.hpp"

struct Prizm : Module {
	enum ParamIds {
		PITCH_PARAM,
		A_WAVE_TOGGLE,
		B_WAVE_TOGGLE,
		C_WAVE_TOGGLE,
		D_WAVE_TOGGLE,
		A_WAVE_MIX,
		B_WAVE_MIX,
		C_WAVE_MIX,
		D_WAVE_MIX,
		KEY_TRACK_AMOUNT,
		NUM_PARAMS
	};
	enum InputIds {
		MOD_A_INPUT,
		V_OCT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		MAIN_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(PHASE_LIGHT, 3),
		NUM_LIGHTS
	};

	class WaveformGenerator {
	public:
		/**
		 * Generate the current waveform value
		 * @param phase -0.5 <= phase < 0.5
		 */
		virtual float getWavepoint(float phase);
	};

	class SineWaveformGenerator : public WaveformGenerator {
		float getWavepoint(float phase) {
			return std::sin(2.f * M_PI * phase);
		}
	};

	class Oscillator {
	private:
		WaveformGenerator& m_generator;
	public:
		Oscillator(WaveformGenerator& generator) : m_generator(generator) {}

		void setWaveformGenerator(WaveformGenerator& generator) { this->m_generator = generator; }
	};

	float sineWave(float phase) {
		return std::sin(2.f * M_PI * phase);
	}

	float subWave(float phase) {
		return std::sin(2.f * M_PI * phase / 4);
	}

	float squareWave(float phase) {
		// if (phase > -1e-4 && phase < 1e-4) {
		// 	return phase / 1e-4;
		// }
		return phase > 0 ? 1 : -1;
	}

	float crazyWave(float phase) {
		return (sineWave(phase) + sineWave(phase * 2) + sineWave(phase + 5/7)) / 3;
	}

	Prizm() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(A_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle A");
		configParam(B_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle B");
		configParam(C_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle C");
		configParam(D_WAVE_TOGGLE, 0.0, 1.0, 0.0, "Wave Toggle D");
	}

	float pythagoras(float a, float b, float c, float d) {
		float sum = 0;
		sum += std::pow(a, 2);
		sum += std::pow(b, 2);
		sum += std::pow(c, 2);
		sum += std::pow(d, 2);
		return std::pow(sum, 1.0 / 4);
	}

	float phase = 0.f;

	void normalize(float& a, float& b, float& c, float& d) {
		float sum = a + b + c + d;
		if (sum < 1e-3) { return; }
		float div = 1.0 / sum;
		a = a * div;
		b = b * div;
		c = c * div;
		d = d * div;
	}

	void process(const ProcessArgs& args) override {
		float pitch = inputs[V_OCT_INPUT].getVoltage();
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

		// Accumulate the phase
		phase += freq * args.sampleTime;
		if (phase >= 0.5f)
			phase -= 1.f;


		// Compute the sine output
		float sineVal = sineWave(phase);
		float sqreVal = squareWave(phase);
		float subVal = subWave(phase);
		float crazyVal = crazyWave(phase);

		float mix_a = clamp(params[A_WAVE_MIX].getValue(), 0.0, 1.0);
		float mix_b = clamp(params[B_WAVE_MIX].getValue(), 0.0, 1.0);
		float mix_c = clamp(params[C_WAVE_MIX].getValue(), 0.0, 1.0);
		float mix_d = clamp(params[D_WAVE_MIX].getValue(), 0.0, 1.0);

		float out = pythagoras(mix_a * sineVal, mix_b * sqreVal, mix_c * subVal, mix_d * crazyVal);

		if (outputs[MAIN_OUTPUT].isConnected()) {
			outputs[MAIN_OUTPUT].setVoltage(5.f * out);
		}
	}
};

struct WaveformPicker : SVGSwitch {
	WaveformPicker() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch-Sin.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch-Sqr.svg")));
	}
};

struct PrizmWidget : ModuleWidget {
	PrizmWidget(Prizm* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Prizm.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

		addParam(createParam<RoundBlackKnob>(Vec(11, 55), module, Prizm::A_WAVE_MIX));
		addParam(createParam<RoundBlackKnob>(Vec(172, 55), module, Prizm::B_WAVE_MIX));
		addParam(createParam<RoundBlackKnob>(Vec(11, 220), module, Prizm::C_WAVE_MIX));
		addParam(createParam<RoundBlackKnob>(Vec(172, 220), module, Prizm::D_WAVE_MIX));

		addParam(createParam<WaveformPicker>(Vec(3, 91), module, Prizm::A_WAVE_TOGGLE));
		addParam(createParam<WaveformPicker>(Vec(148, 91), module, Prizm::B_WAVE_TOGGLE));
		addParam(createParam<WaveformPicker>(Vec(3, 256), module, Prizm::C_WAVE_TOGGLE));
		addParam(createParam<WaveformPicker>(Vec(148, 256), module, Prizm::D_WAVE_TOGGLE));

		addInput(createInput<PJ301MPort>(Vec(12, 310), module, Prizm::V_OCT_INPUT));
		addInput(createInput<PJ301MPort>(Vec(95, 310), module, Prizm::MOD_A_INPUT));

		addOutput(createOutput<PJ301MPort>(Vec(175, 310), module, Prizm::MAIN_OUTPUT));
	}
};

Model* modelPrizm = createModel<Prizm, PrizmWidget>("Prizm");
