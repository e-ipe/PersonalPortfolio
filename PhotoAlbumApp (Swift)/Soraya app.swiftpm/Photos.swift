import SwiftUI

struct Photos: View {
    let columns = [ GridItem(.adaptive(minimum: 175), spacing: 5)]
    let nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
    var body : some View {
        VStack {
            Text("Soraya baby")
                .font(.system(.title, design: .rounded))
            ScrollView {
                LazyVGrid(columns: columns, spacing: 5) {
                    ForEach (nums, id: \.self){ i in
                        Image ("Soraya\(i)")
                            .resizable()
                            .scaledToFit()
                            .padding()
                    }
                }
            }
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(.pink)
    }
}

struct Photos_Previews : PreviewProvider {
    static var previews : some View {
        Photos()
    }
}

