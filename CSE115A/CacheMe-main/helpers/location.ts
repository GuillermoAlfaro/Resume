import * as Location from 'expo-location';

export default async function getLocation() {
    let { status } = await Location.requestForegroundPermissionsAsync();
    if (status !== 'granted') {  
        return;
      }
    
    let location = await Location.getLastKnownPositionAsync();
    return location;
}