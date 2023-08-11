import MapView, { Circle, Marker } from 'react-native-maps';
import { StyleSheet, View, ActivityIndicator } from 'react-native';
import { PROVIDER_GOOGLE } from 'react-native-maps';
import { useEffect, useState } from 'react';
import getLocation from '../../helpers/location';
import React from 'react';
import Location from 'expo-location';
import { useLocalSearchParams, useRouter, useSearchParams } from 'expo-router';
import MapViewDirections from 'react-native-maps-directions'

// Firebase
import { auth, db } from '../../firebase';
import { GeoPoint, Timestamp, collection, getDocs, orderBy, query } from 'firebase/firestore';


const cache = {
  __id: "",
  __imageId: "",
  __userId: "",
  _createdAt: new Timestamp(0, 0),
  caption: "",
  location: new GeoPoint(0, 0),
  numComments: 0,
  numLikes: 0,
  reported: false
}

type CacheData = {
  __id: string; 
  __imageId: string; 
  __userId: string; 
  _createdAt: Timestamp; 
  caption: string; 
  location: GeoPoint; 
  numComments: number; 
  numLikes: number; 
  reported: boolean;
}


export default function Map() {
  const [location, setLocation] = useState<Location.LocationObject | null | undefined>();
  const [loading, setLoading] = useState(true);
  const [postsToRender, setPostsToRender] = useState([cache]);
  const router = useRouter();
  const params = useLocalSearchParams();
  const { latitude, longitude } = useSearchParams();
  const mapRef = React.createRef();
  const fetchPosts = async () => {
    const postsList:CacheData[] = [];
    const cacheRef = collection(db, "cache");
    const q = query(cacheRef, orderBy('_createdAt', 'desc'));
    const querySnapshot = await getDocs(q);

    querySnapshot.forEach((cache) => {
        const {
            __id,
            __imageId,
            __userId,
            _createdAt,
            caption,
            location,
            numComments,
            numLikes,
            reported
        } = cache.data();
        postsList.push({
            __id,
            __imageId,
            __userId,
            _createdAt,
            caption,
            location,
            numComments,
            numLikes,
            reported
        });
    });
    setPostsToRender(postsList);
}

useEffect(() => {
  fetchPosts();
})

  useEffect(() => {
    (async () => {

        const loc = await getLocation()
        setLocation(loc);
        setLoading(false);
    })();
}, []);
function toRadians(degrees: number) {
  return degrees * (Math.PI / 180);
}

function toDegrees(radians: number) {
  return radians * (180 / Math.PI);
}

function calculateMidpoint(lat1: number, lon1: number, lat2: number, lon2: number) {
  // Convert all latitudes/longitudes from decimal degrees to radians
  lat1 = toRadians(lat1);
  lon1 = toRadians(lon1);
  lat2 = toRadians(lat2);
  lon2 = toRadians(lon2);

  // calculate the differences between the coordinates
  var dLon = lon2 - lon1;

  // Calculate Bx and By
  var Bx = Math.cos(lat2) * Math.cos(dLon);
  var By = Math.cos(lat2) * Math.sin(dLon);

  // Calculate the midpoint coordinates
  var midLat = Math.atan2(
      Math.sin(lat1) + Math.sin(lat2),
      Math.sqrt((Math.cos(lat1) + Bx) * (Math.cos(lat1) + Bx) + By * By)
  );
  var midLon = lon1 + Math.atan2(By, Math.cos(lat1) + Bx);

  // Convert the midpoint coordinates to degrees
  midLat = toDegrees(midLat);
  midLon = toDegrees(midLon);

  return { latitude: midLat, longitude: midLon };
}

function getDistanceInLatDegrees(lat1: number, lon1: number, lat2: number, lon2: number) {
  var earthRadius = 6371; // Radius of the earth in km
  var dLat = toRadians(lat2-lat1);  
  var dLon = toRadians(lon2-lon1); 
  var a = 
    Math.sin(dLat/2) * Math.sin(dLat/2) +
    Math.cos(toRadians(lat1)) * Math.cos(toRadians(lat2)) * 
    Math.sin(dLon/2) * Math.sin(dLon/2); 
  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)); 
  var d = earthRadius * c; // Distance in km

  var kmPerDegree = 111.111; // average km per degree of latitude
  var dInLatDegrees = d / kmPerDegree;
  
  return dInLatDegrees;
}


  if(loading){
    return(
      <View style={[styles.container, styles.map]}>
        <ActivityIndicator size="large" />
      </View>
    )
  }
  return (
    <View style={styles.container}>
      <MapView
      style={styles.map}
      showsUserLocation={true}
      provider={PROVIDER_GOOGLE}
      region={{
        latitude: latitude ? calculateMidpoint(parseFloat(latitude as string) as number, parseFloat(longitude as string) as number, location?.coords.latitude as number, location?.coords.longitude as number).latitude : location?.coords.latitude as number,
        longitude: longitude ? calculateMidpoint(parseFloat(latitude as string) as number, parseFloat(longitude as string) as number, location?.coords.latitude as number, location?.coords.longitude as number).longitude : location?.coords.longitude as number,
        latitudeDelta: latitude ? getDistanceInLatDegrees(parseFloat(latitude as string) as number, parseFloat(longitude as string) as number, location?.coords.latitude as number, location?.coords.longitude as number) * 1.9 : .28,
        longitudeDelta: longitude ? getDistanceInLatDegrees(parseFloat(latitude as string) as number, parseFloat(longitude as string) as number, location?.coords.latitude as number, location?.coords.longitude as number) * 0.2 : .28,
      }}>
        <Circle
        fillColor='rgba(1,1,1,0.5)'
        radius={5000}
        center={
          {latitude: location?.coords.latitude as number,
          longitude: location?.coords.longitude as number}
        }
        ></Circle>
        {latitude && longitude && <MapViewDirections
        strokeColor='blue'
        strokeWidth={5}
        apikey='AIzaSyCuImeYX2zaOvY2mQ4bRMtixQ-UsMCgHI8'
        origin={{
          latitude: location?.coords.latitude as number,
          longitude: location?.coords.longitude as number
        }}
        destination={{
          latitude: parseFloat(latitude as string) as number,
          longitude: parseFloat(longitude as string) as number,
        }}
        />}
        {postsToRender.map((post) => { // Going through every post and returning a marker object for it

          return (
            <Marker
            pinColor={post.__userId == auth.currentUser?.uid ? 'green' : 'red'}
            key={post.__id}
            coordinate={{
              latitude: post.location.latitude as number,
              longitude: post.location.longitude as number
            }}
             onPress={() => {
              const { postId = post.__id } = params;
              router.push({ pathname: '/postPageEX', params: { postId }})
             }} // Sends you to the page for the specific route
             >
              
            </Marker>
          )
        })}
      </MapView>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
  },
  map: {
    width: '100%',
    height: '100%',
  },
});