/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

#include <random>

#include "plugin.hpp"
#include "math.hpp"
#include "types.hpp"
#include "oscillator.hpp"

#define NUM_OSCILLATORS 4

/**
 * Widget switch to toggle waveform for an oscillator
 * 
 * @note for performance reasons, this should have the same order as @see{WavetableOscillator::Wavetable}
 */
struct WaveformPicker : SvgSwitch {
	WaveformPicker() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch/Sine.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch/Square.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch/Crazy-Sine.svg")));
	}
};

class Prizm : public Module {
public:
	enum ParamIds {
		PITCH_PARAM,
		A_WAVE_SHAPE,
		B_WAVE_SHAPE,
		C_WAVE_SHAPE,
		D_WAVE_SHAPE,
		A_INTENSITY,
		B_INTENSITY,
		C_INTENSITY,
		D_INTENSITY,
		A_FINE,
		B_FINE,
		C_FINE,
		D_FINE,
		A_COARSE,
		B_COARSE,
		C_COARSE,
		D_COARSE,
		MORPH_STYLE,
		MASS,
		SMOOTH,
		NUM_PARAMS
	};
	enum InputIds {
		A_INTENSITY_MOD,
		B_INTENSITY_MOD,
		C_INTENSITY_MOD,
		D_INTENSITY_MOD,
		MASS_MOD,
		SMOOTH_MOD,
		V_OCT,
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

	Prizm() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		configParam(A_WAVE_SHAPE, 0.0, 5.0, 0.0, "Wave Toggle A");
		configParam(B_WAVE_SHAPE, 0.0, 5.0, 0.0, "Wave Toggle B");
		configParam(C_WAVE_SHAPE, 0.0, 5.0, 0.0, "Wave Toggle C");
		configParam(D_WAVE_SHAPE, 0.0, 5.0, 0.0, "Wave Toggle D");

		this->m_oscillators[0].setWavetable(WavetableOscillator::Wavetable::SINE);
		this->m_oscillators[1].setWavetable(WavetableOscillator::Wavetable::SQUARE);
		this->m_oscillators[2].setWavetable(WavetableOscillator::Wavetable::CRAZY_SINE);
		this->m_oscillators[3].setWavetable(WavetableOscillator::Wavetable::FM_ONE);
	}

	/**
	 * Process a new sample
	 */
	void process(const ProcessArgs& args) override {
		accumulatePhase(args);

		for (size_t i = 0; i < 4; i++) {
			auto oscillator = m_oscillators[i];

			/**
			 * current un-normalized mix value = switch value + cv input value
			 */
			m_mixValues[i] = clamp(
				inputs[A_INTENSITY_MOD + i].getVoltage() + params[A_INTENSITY + i].getValue(),
				-5.0f, 5.0f
			);

			m_wavepoints[i] = oscillator.getNextWavepoint(m_phase, 0.5f, true);
		}

		normalize(m_mixValues, NUM_OSCILLATORS);

		for (size_t i = 0; i < NUM_OSCILLATORS; i++) {
			m_wavepoints[i] *= m_mixValues[i];
		}

		float output = tanh(pythagoras(m_wavepoints, NUM_OSCILLATORS));

		if (outputs[MAIN_OUTPUT].isConnected()) {
			outputs[MAIN_OUTPUT].setVoltage(5.0f * output);
		}
	}
private:
	float m_phase = 0.0f;

	float m_wavepoints [NUM_OSCILLATORS] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float m_mixValues  [NUM_OSCILLATORS] = { 0.0f, 0.0f, 0.0f, 0.0f };

	WavetableOscillator m_oscillators [4] = { 
		WavetableOscillator(), 
		WavetableOscillator(), 
		WavetableOscillator(), 
		WavetableOscillator() 
	};

	inline hz getFrequency() {
		float pitch = inputs[V_OCT].getVoltage();

		return dsp::FREQ_C4 * std::pow(2.0f, pitch);
	}

	inline void accumulatePhase(const ProcessArgs& args) {
		m_phase += this->getFrequency() * args.sampleTime;

		if (m_phase >= 0.5f) {
			m_phase -= 1.f;
		}
	}
};

struct PrizmWidget : ModuleWidget {
	const float scale_x = 2.74;
	const float scale_y = 2.6;

	PrizmWidget(Prizm* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Prizm.svg")));

		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

		addParam(createParam<RoundSmallBlackKnob>(Vec(scale_x*100.1, scale_y*46.7), module, Prizm::A_INTENSITY));
		addParam(createParam<RoundSmallBlackKnob>(Vec(172, 55), module, Prizm::B_INTENSITY));
		addParam(createParam<RoundSmallBlackKnob>(Vec(11, 220), module, Prizm::C_INTENSITY));
		addParam(createParam<RoundSmallBlackKnob>(Vec(172, 220), module, Prizm::D_INTENSITY));

		// addParam(createParam<WaveformPicker>(Vec(3, 91), module, Prizm::A_WAVE_SHAPE));
		// addParam(createParam<WaveformPicker>(Vec(148, 91), module, Prizm::B_WAVE_SHAPE));
		// addParam(createParam<WaveformPicker>(Vec(3, 256), module, Prizm::C_WAVE_SHAPE));
		// addParam(createParam<WaveformPicker>(Vec(148, 256), module, Prizm::D_WAVE_SHAPE));

		addParam(createParam<RoundSmallBlackKnob>(Vec(scale_x*100.1, scale_y*40), module, Prizm::C_WAVE_SHAPE));
		addParam(createParam<RoundSmallBlackKnob>(Vec(scale_x*100.1, scale_y*87.7), module, Prizm::D_WAVE_SHAPE));

		addParam(createParam<RoundSmallBlackKnob>(Vec(scale_x*134.5, scale_y*33), module, Prizm::MORPH_STYLE));
		addParam(createParam<RoundSmallBlackKnob>(Vec(scale_x*134.5, scale_y*93.7), module, Prizm::SMOOTH));

		addInput(createInput<PJ301MPort>(Vec(12, 310), module, Prizm::V_OCT));
		addInput(createInput<PJ301MPort>(Vec(scale_x*134.5, scale_y*61.6), module, Prizm::MASS_MOD));
		addInput(createInput<PJ301MPort>(Vec(172, 182), module, Prizm::SMOOTH_MOD));

		addInput(createInput<PJ301MPort>(Vec(11, 125), module, Prizm::A_INTENSITY_MOD));
		addInput(createInput<PJ301MPort>(Vec(172, 125), module, Prizm::B_INTENSITY_MOD));
		addInput(createInput<PJ301MPort>(Vec(11, 182), module, Prizm::C_INTENSITY_MOD));
		addInput(createInput<PJ301MPort>(Vec(172, 182), module, Prizm::D_INTENSITY_MOD));

		addOutput(createOutput<PJ301MPort>(Vec(175, 310), module, Prizm::MAIN_OUTPUT));
	}
};

Model* modelPrizm = createModel<Prizm, PrizmWidget>("Prizm");