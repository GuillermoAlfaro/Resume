// This is the drop down menu for privacy in post creation page.
import React, { useState } from 'react';
import { StyleSheet } from 'react-native';
import { SelectCountry as SelectPrivacy } from 'react-native-element-dropdown';

const local_data = [
  { value: '1', lable: 'Public', },
  { value: '2', lable: 'Private', },
  { value: '3', lable: 'Friends', }
];

const SelectPrivacyScreen = (_props: any) => {
  const [privacy, setPrivacy] = useState('1');

  return (
    <SelectPrivacy
      style={styles.dropdown}
      selectedTextStyle={styles.selectedTextStyle}
      placeholderStyle={styles.placeholderStyle}
      itemContainerStyle={styles.containerStyle}
      containerStyle={styles.containerStyle}
      maxHeight={200}
      value={privacy}
      data={local_data}
      valueField="value"
      labelField="lable"
      imageField="image"
      placeholder="Select Privacy"
      searchPlaceholder="Search..."
      onChange={e => {
        setPrivacy(e.value);
      }}
    />
  );
};

export default SelectPrivacyScreen; 

const styles = StyleSheet.create({
  dropdown: {
    margin: 0,
    height: 35,
    width: 85,
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
    fontSize: 15,
  },
  selectedTextStyle: {
    fontSize: 16,
  },
  containerStyle: {
    borderRadius: 10
  },
});