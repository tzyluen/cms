/*
* Copyright 2008 Google Inc. 
* Licensed under the Apache License, Version 2.0:
*  http://www.apache.org/licenses/LICENSE-2.0
*/
package ui {

import mx.containers.TabNavigator;
import mx.containers.VBox;
import mx.controls.TextArea;
import mx.core.UIComponent;

/**
 * InfoWindowSprite consists of several ellipses arranged in a 'thought bubble'
 * manner, the largest of which contains an embedded image and a circular
 * close button.
 * It can dispatch an Event instance (type: "close"), which the user can listen
 * for and use to call map.closeInfoWindow();
 */
public class InfoWindowTabbedComponent extends UIComponent {
              
  public function InfoWindowTabbedComponent(tabData:Object) {
    // Add body text
    var tabNavigator:TabNavigator = new TabNavigator();
    tabNavigator.width = 290;
    tabNavigator.height = 150;
    
      /*crimeRecord['UCRCategories'] = crime.UCRCategories;
      crimeRecord['type'] = crime.crimeCategory;
      crimeRecord['business'] = crime.business;
      crimeRecord['startDate'] = crime.startDate;
      crimeRecord['description'] = crime.description;
      crimeRecord['county'] = crime.county;
      crimeRecord['location'] = crime.location;
      crimeRecord['city'] = crime.city;
      crimeRecord['state'] = crime.state;
      crimeRecord['zip'] = crime.zip;
      crimeRecord['lat'] = crime.lat;
      crimeRecord['lon'] = crime.lon;*/
    var info:String = (tabData['Cluster'] != null) ? ("<b>Cluster: </b>" + tabData['Cluster'] + "<br/>") : "";
    info += "<b>UCRCategory: </b>" + tabData['UCRCategories'] + "<br/>" +
                      "<b>Type: </b>" + tabData['type'] + "<br/>" +
                      "<b>Date: </b>" + tabData['startDate'] + "<br/>" +
                      "<b>Business: </b>" + tabData['business'] + "<br/>" +
                      "<b>County: </b>" + tabData['county'] + "<br/>" +
                      "<b>Location: </b>" + tabData['location'] + "<br/>" +
                      "<b>City: </b>" + tabData['city'] + "<br/>" +
                      "<b>State: </b>" + tabData['state'] + "<br/>" +
                      "<b>Zip: </b>" + tabData['zip'] + "<br/>" + 
                      "<b>Latitude: </b>" + tabData['lat'] + "<br/>" +
                      "<b>Longitude: </b>" + tabData['lon'] + "<br/>";
    /*tabNavigator.addChild(createTab("uno", "Lights go down <br> It's dark <br> The jungle is your head <br> Can't rule your heart"));
    tabNavigator.addChild(createTab("dos", "I'm feeling so much stronger <br/> Than I thought <br/> Your eyes are wide <br/> "));
    tabNavigator.addChild(createTab("tres", "And though your soul <br/> it can't be bought <br/> your mind can wander"));
    tabNavigator.addChild(createTab("catorce!", "Hello, Hello  <br/> I'm at a place called vertigo <br/>  It's everything I wish I didn't know <br/> Except you give me something I can feel"));*/
    
    tabNavigator.addChild(createTab("Info", info));
    tabNavigator.addChild(createTab("Description", tabData['description']));
    
    if (tabData['convictId'] != null) {
      var convictInfo:String = "<b>Convict ID: </b>" + tabData['convictId'] + "<br/>" +
                               "<b>Name: </b>" + tabData['fname'] + " " +tabData['middle'] + " " + tabData['lname'] + "<br/>" +
                               "<b>Age: </b>" + tabData['age'] + "<br/>" +
                               "<b>Address: </b>" + tabData['address'] + "<br/>";
      trace(convictInfo);
      tabNavigator.addChild(createTab("Convict", convictInfo));
    } else if (tabData['suspectId'] != null) {
      var suspectInfo:String = "<b>Suspect ID: </b>" + tabData['suspectId'] + "<br/>" +
                               "<b>Age: </b>" + tabData['age'] + "<br/>";
      if (tabData['raceId'] != null) {
        var race:String = "";
        switch (tabData['raceId']) {
          case 0:
            race = 'Unknown';
          case 20:
            race = 'Caucasian';
            break;
          case 25:
            race = 'Latino';
            break;
          case 30:
            race = 'Native American';
            break;
          case 35:
            race = 'Black';
            break;
          case 40:
            race = 'Asian';
            break;
          case 45:
            race = 'Eskimo';
            break;
          default:
            race = 'Unknown';
            break;
        }
        suspectInfo += "<b>Race : </b>" + race + "<br/>";
      }
      
      if (tabData['weaponId'] != null) {
        var weapon:String = "";
        switch (tabData['weaponId']) {
          case 0:
            weapon = 'Unknown';
            break;
          case 20:
            weapon = 'Pistol';
            break;
          case 25:
            weapon = 'Short Gun';
            break;
          case 30:
            weapon = 'Knife';
            break;
          case 35:
            weapon = 'Spears';
            break;
          case 40:
            weapon = 'Sword';
            break;
          case 45:
            weapon = 'Bat';
            break;
          case 50:
            weapon = 'Nunchaku';
            break;
          default:
            weapon = 'Unknown';
            break;
        }
        suspectInfo += "<b>Weapon : </b>" + weapon + "<br/>";
      }
      trace(suspectInfo);
      tabNavigator.addChild(createTab("Suspect", suspectInfo));
    }
    
    addChild(tabNavigator);
    
    cacheAsBitmap = true;
  }
  
  public function createTab(label:String, text:String):VBox {
  	var tab:VBox = new VBox();
  	tab.label = label;
  	var inside:TextArea = new TextArea();
  	
  	inside.editable = false;
  	inside.selectable = false;
  	inside.width = 280;
  	inside.height = 100;
  	inside.htmlText = text;
  	inside.setStyle("borderStyle", "none");
  	tab.addChild(inside);
  	return tab;
  }
}

}
