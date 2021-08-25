/**
 * @author: github.com/velechva
 * 
 * This software is licensed under the GNU General Public License v3.
 * See LICENSE.md for more details
 */

#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelPrizm);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
