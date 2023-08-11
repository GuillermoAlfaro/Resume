// This is the drop down menu for post radius in creation page.
import React, { useState } from 'react';
import { StyleSheet } from 'react-native';
import { SelectCountry as SelectRadius } from 'react-native-element-dropdown';

const local_data = [
  { value: '5', lable: '5 km', },
  { value: '15', lable: '15 km', },
  { value: '25', lable: '25 km', },
  { value: '50', lable: '50 km', }, 
];

const SelectRadiusScreen = (_props: any) => {
  const [radius, setRadius] = useState('5');

  return (
    <SelectRadius
      style={styles.dropdown}
      selectedTextStyle={styles.selectedTextStyle}
      placeholderStyle={styles.placeholderStyle}
      itemContainerStyle={styles.containerStyle}
      containerStyle={styles.containerStyle}
      itemTextStyle={styles.containerText}
      maxHeight={200}
      value={radius}
      data={local_data}
      valueField="value"
      labelField="lable"
      imageField="image"
      placeholder="Select Radius"
      onChange={e => {
        setRadius(e.value);
      }}
    />
  );
};

export default SelectRadiusScreen;

const styles = StyleSheet.create({
  dropdown: {
    margin: 0,
    height: 35,
    width: 98,
    backgroundColor: 'white',
    borderRadius: 10,
    paddingHorizontal: 8,
    
  },
  imageStyle: {
    width: 24,
    height: 24,
    borderRadius: 12,
  },
  placeholderStyle: {
    fontSize: 16,
  },
  selectedTextStyle: {
    fontSize: 16,
  },
  containerStyle: {
    borderRadius: 10,
  },
  containerText: {
    textAlign: "center",
  }
});