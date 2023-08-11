import { distanceBetween } from "geofire-common";
import getLocation from "./location";
import { useLocalSearchParams, useRouter } from "expo-router";
import { auth } from "../firebase";
import { Router } from "@react-navigation/routers";

export async function getUserDistanceFromPost(postLat: number, postLong: number) {
    const loc = await getLocation();
    let currUserLat = loc?.coords.latitude as number;
    let currUserLong = loc?.coords.longitude as number;
    return Math.round(distanceBetween([currUserLat, currUserLong], [postLat, postLong]));
}

export function scrambleText(input: string): string {
    // Convert string to array
    let characters = Array.from(input);
  
    // Use the Fisher-Yates algorithm to scramble the array
    for (let i = characters.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [characters[i], characters[j]] = [characters[j], characters[i]];
    }
  
    // Convert the array back to a string
    return characters.join('');
  }