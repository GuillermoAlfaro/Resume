import { Tabs } from "expo-router";
import { AntDesign } from '@expo/vector-icons';
import { FontAwesome5 } from '@expo/vector-icons';
import { colors } from "../../components/colors";
const {primary} = colors;
/**
 * Icons from: https://icons.expo.fyi/
 */

export default function AppLayout() {
    return (
      <Tabs
      screenOptions={{header: () => null}}>
        <Tabs.Screen
          name="feed"
          options={{
            tabBarIcon: ({ focused }) => <AntDesign name="home" size={32} color={focused ? primary : "black"} />,
            title: "",
          }}
        />
        <Tabs.Screen
          name="map"
          options={{
            tabBarIcon: ({ focused }) => <FontAwesome5 name="map" size={32} color={focused ? primary : "black"} />,
            title: "",
          }}
        />
        <Tabs.Screen
          name="post"
          options={{
            tabBarIcon: ({ focused }) => <AntDesign name="plussquareo" size={32} color={focused ? primary : "black"} />,
            title: "",

          }}
        />
        <Tabs.Screen
          name="notifications"
          options={{
            tabBarIcon: ({ focused }) => <FontAwesome5 name="bell" size={32} color={focused ? primary : "black"} />,
            title: "",
          }}
        />
        <Tabs.Screen
          name="profile"
          options={{
            tabBarIcon: ({ focused }) => <AntDesign name="user" size={32} color={focused ? primary : "black"} />,
            title: "",
          }}
        />
      </Tabs>
    );
  }